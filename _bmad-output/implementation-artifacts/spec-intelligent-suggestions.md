---
title: 'Wire IntelligentSuggestionSystem into IDE GUI'
type: 'feature'
created: '2026-09-17'
status: 'done'
route: 'oneshot'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** `IntelligentSuggestionSystem` (632-line rule-based engine: bug detection, refactoring, optimization, best-practice, docs, tests, architectural analysis) is fully implemented but never wired into the GUI. `IDEGUI::UpdateIntelligentSuggestions()` pushes one hardcoded placeholder suggestion; `LearnFromSuggestion()` never calls `LearnFromFeedback`; `UpdateIntelligentSuggestions` is never invoked anywhere. Also `IntelligentSuggestionSystem`'s three AI-module pointers (`textEmbedding`, `nlpPipeline`, `suggestionModel`) are declared but never initialized — uninitialized garbage members.

**Approach:** Instantiate the suggestion system in the IDE GUI (file-scope static, matching the existing `g_DevSystem` convention), drive `UpdateIntelligentSuggestions()` from the real editor buffer + active tab + cursor position with content-change debounce, wire `LearnFromSuggestion()` to `LearnFromFeedback`, and null-initialize the unused AI pointers. No third-party deps; comments in Traditional Chinese.

</frozen-after-approval>

## Implementation Notes

- Wired `g_SuggestionSystem` (file-scope static, matching `g_DevSystem` convention): allocated in `IDEGUI` ctor, deleted in `Shutdown`.
- `UpdateIntelligentSuggestions()` analyzes the **active tab's** `buffer`/`filePath`/`currentLine`/`currentColumn` — the real editor edits `tab.buffer`, not the legacy `state.editorBuffer` (which stays empty and would have made the feature dead code).
- True debounce: content+path hash change stamps `s_lastEditTime`; analysis runs only after a 500ms quiet period, so mid-typing doesn't clobber the visible list.
- Apply now appends into `tab.buffer`+`tab.content` and marks the tab modified; applied suggestions are erased from the list like dismissed ones.
- Dismiss records the suggestion title in `g_DismissedSuggestionTitles` (id changes every generation, so title is the suppression key); suppressed titles are filtered out of future passes.
- Panel auto-shows only when the suggestion-set signature changes — a user-closed panel stays closed for identical results.
- `Suggestion::id` is now assigned as `<typeKey>_<counter>` (`nextSuggestionId` member); `LearnFromFeedback` parses with `rfind('_')` so multi-underscore type keys (`bug_fix`, `code_completion`…) resolve, and rejection lowers the type weight (floor 0.1) — the weights feed `CalculateConfidence`, so feedback now actually steers ranking.
- Fixed latent analyzer bug now that it is live: magic-number check scanned `analysis.filePath` instead of code; `CodeAnalysis` gained a `codeText` field and the regex scans it.
- Unused AI-module pointers (`textEmbedding`, `nlpPipeline`, `suggestionModel`) initialized to nullptr.

## Review Triage Log

- high — analyzed `state.editorBuffer` instead of active `tab.buffer`: feature could never fire during real editing. Fixed (reads active tab).
- high — `ApplySuggestion` wrote to the dead `editorBuffer`; applied code invisible and falsely marked file dirty. Fixed (writes `tab.buffer`/`tab.content`).
- medium — `state.currentLine/currentColumn/currentFile` never populated; passed `(1,1,"")` forever. Fixed (uses tab fields).
- medium — no `Suggestion::id` assigned and `LearnFromFeedback` parsed with `find('_')` so weights never matched even with ids. Fixed (`<typeKey>_<counter>` + `rfind`).
- medium — rejected feedback was a no-op. Fixed (weight decrease, floor 0.1).
- medium — dismissed suggestions resurrected on next analysis. Fixed (title suppression set).
- medium — 500ms throttle replaced buffer mid-typing. Fixed (quiet-period debounce).
- medium — magic-number regex scanned `filePath` not code (pre-existing, now live). Fixed via `CodeAnalysis::codeText`.
- medium — Apply left the entry in the list, allowing duplicate insertion. Fixed (erase on Apply).
- low — disabling `intelligentSuggestionsEnabled` left a stale interactive panel. Fixed (clears state).
- low — panel re-opened after explicit user Close on identical results. Fixed (signature check).
- low — hash ignored filePath/tab identity. Fixed (path folded into hash input).
- low rejected — per-frame 8KB hash cost is trivial (microseconds); dirty-flag plumbing not worth it.
- deferred — synchronous analysis + per-call `std::regex` construction on render thread (recorded in deferred-work.md).
- deferred — file-scope raw-pointer AI globals lifetime hazards, matches existing convention (recorded in deferred-work.md).
