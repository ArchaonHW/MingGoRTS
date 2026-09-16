---
title: 'IDE Intelligent Development Assistant — own in-process AI agent codegen'
type: 'feature'
created: '2026-09-16'
status: 'done'
route: 'dispatch'
baseline_commit: 'd5eff759dd45e73a4caa47ab7865a33446ff6d24'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** The IDE's Development Assistant panel is wired on the surface but dead underneath — `g_DevSystem->Initialize()` is never called, `GenerateCode` only knows how to call an (entirely mocked) LLM client, and generation runs synchronously on the UI thread. The feature cannot produce any real output.

**Approach:** Power the assistant with the engine's OWN AI agent stack — a local, deterministic code-generation pipeline inside `IntelligentDevelopmentSystem` (NLP intent parsing → KnowledgeGraph project context → template/rule-based C++ synthesis), so it works offline with no external LLM. External LLM clients stay pluggable but are not required. Generation runs off the UI thread.

**Decisions:**
- BACKEND = own in-process agent pipeline (no HTTP, no API key required for the core path).
- CONFIG = both: `POTATO_LLM_*` env vars provide defaults; Settings UI fields override them at runtime.

## Boundaries & Constraints

**Always:**
- Must compile under both MSVC and MinGW; no new third-party dependencies (no HTTP lib, no JSON lib — the pipeline is in-process).
- Fixed `char` state buffers written via `strncpy`/`memset` + explicit NUL; banned functions (`gets|strcpy|strcat|sprintf|vsprintf|scanf`) fail CI.
- UI thread never blocks on generation; `developmentProcessing` gates re-entry and drives the visible "Processing..." state.
- No secrets in code — env-var names only, never values.

**Never:**
- No changes under `external/` or `build/`.
- Do not auto-insert generated code into `editorBuffer` — response goes to the response view only (remove the existing append at ~IDEGUI.cpp:2621).
- Do not rip out `ILLMClient`/`LLMManager` — they remain as the optional external-provider path.
- Out of scope: `DevelopmentAssistant::Ask`/RAG wiring, `ExplainCode`, other panels, and real HTTP backends.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| HAPPY_PATH | Non-empty prompt e.g. "create a class that ..." | Synthesized C++ code in `developmentResponse`; Processing shown during work | N/A |
| UNRECOGNIZED | Prompt the pipeline can't map to a template | Best-effort scaffold or clear "cannot generate" message — never garbage | `CodeGenerationResult.success=false` + error text shown |
| EMPTY_PROMPT | Empty/whitespace prompt | Generate is a no-op (disabled or early return) | N/A |
| LONG_RESPONSE | Output > 8191 chars | Truncated + `...[truncated]` marker | `strncpy` bounded copy |
| DOUBLE_SUBMIT | Generate clicked while processing | Ignored until current run completes | Flag guard |
| WORKER_EXIT | Generate clicked during/after partial failure | Worker result consumed once; flag always cleared | Poll resets state on exception |

</frozen-after-approval>

## Code Map

- `AI/IntelligentDevelopmentSystem.h:135-` / `.cpp:101-149` — `GenerateCode(spec, lang, ctx)`; today delegates to `llmClient` and fails when null (`:111-114`). Add the local pipeline here so a null client no longer means failure. `ExtractCodeFromResponse` (`:643-654`) reusable for the LLM path.
- `AI/IntelligentDevelopmentSystem.h:25-29` `DevTaskType`, task queue (`:150-154`) — existing task plumbing to reuse, not bypass.
- `AI/NaturalLanguageProcessing.h/.cpp` — intent/keyword extraction for prompt → task mapping.
- `AI/KnowledgeGraph.h:78` — project knowledge for context-aware generation (engine class names, conventions).
- `AI/SelfReflection.h:108` — optional post-generation self-check hook; keep wiring minimal (call if already functional, else skip).
- `MingGoRTS_IDE/GUI/IDEGUI.cpp:38-44` — ctor creates `g_LLMManager`, `g_DevSystem`; call `g_DevSystem->Initialize(nullptr, agentManager)` so local pipeline is primary.
- `MingGoRTS_IDE/GUI/IDEGUI.cpp:2604-2634` — `GenerateCodeFromPrompt()`; make async via `std::async`/worker + per-frame poll in render.
- `MingGoRTS_IDE/GUI/IDEGUI.cpp:2756-2822` — `RenderDevelopmentAssistant()`; Generate button + Processing text exist — wire to flag.
- `MingGoRTS_IDE/GUI/IDEGUI.h:178-182` — state fields; add `std::future<CodeGenerationResult>` member.
- `MingGoRTS_IDE/GUI/IDEGUI.cpp` Settings panel + `I18N.h` — provider/model/agent-config fields; read `POTATO_LLM_MODEL`, `POTATO_LLM_PROVIDER`, `POTATO_LLM_BASEURL` env defaults.
- `CMakeLists.txt:307-317` — `MingGoRTS_IDE_GUI` already links everything needed; expected NO changes (verify `KnowledgeGraph`/`SelfReflection`/`NLP` objects are in a linked lib or add to `IntelligentDevelopmentSystem` sources).

## Tasks & Acceptance

**Execution:**
- [x] `AI/IntelligentDevelopmentSystem.cpp` + `.h` -- implement `GenerateLocal(specification, language)` pipeline: NLP intent → template/rule synthesis → optional KnowledgeGraph context; `GenerateCode` tries local pipeline first, falls back to `llmClient` only when configured -- the actual intelligence
- [x] `AI/IntelligentDevelopmentSystem.cpp` -- seed a minimal template set (class, function, system-stub, test-stub) driven by parsed intent -- makes output real instead of mock text
- [x] `MingGoRTS_IDE/GUI/IDEGUI.h` -- add `std::future<CodeGenerationResult>` + settings fields -- async storage + config surface
- [x] `MingGoRTS_IDE/GUI/IDEGUI.cpp` -- init `g_DevSystem` for local pipeline; async `GenerateCodeFromPrompt` with poll-write-back, empty-prompt guard, double-submit guard, truncation marker, editorBuffer append removed -- user-visible behavior
- [x] `MingGoRTS_IDE/GUI/IDEGUI.cpp` Settings + `I18N` -- provider/baseURL/apiKey/model/agent fields; env-var defaults `POTATO_LLM_*` -- runtime configuration
- [x] `CMakeLists.txt` -- only if new .cpp objects aren't already in a linked lib -- keep zero new deps
- [x] `Examples/` small CLI smoke (or extend an existing demo) calling `IntelligentDevelopmentSystem::GenerateCode` with no LLM client -- verifiable without GUI

**Acceptance Criteria:**
- Given the IDE running with no external LLM configured, when a user submits a recognizable codegen prompt, then synthesized C++ appears in the response view and the UI stays responsive.
- Given an unrecognizable prompt, when submitted, then a clear "cannot generate" style message appears — never a hang, crash, or editor mutation.
- Given `cmake --build` on MSVC or MinGW, then `MingGoRTS_IDE_GUI` and the smoke example build clean with no new banned-function use.

## Implementation Notes

## Spec Change Log

## Review Triage Log

## Design Notes

Local pipeline shape (sketch): `ParseIntent(prompt)` → `{taskType, subject, members[]}` → pick template → fill from KnowledgeGraph project context → emit code. Keep the template table data-driven (map of taskType → generator fn) so future templates are one line each.

## Verification

**Commands:**
- `cmake -S . -B build && cmake --build build --config Release` -- `MingGoRTS_IDE_GUI` + smoke example build clean
- `grep -rnE "(gets|strcpy|strcat|sprintf|vsprintf|scanf)\s*\(" MingGoRTS_IDE AI --include='*.cpp' --include='*.h'` -- no new matches

**Manual checks (if no CLI):**
- GUI: Ctrl+D → prompt → response appears, UI responsive; unrecognized prompt → clean error message.
