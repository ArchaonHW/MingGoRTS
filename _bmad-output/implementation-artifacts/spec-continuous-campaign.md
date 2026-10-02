---
title: Persistent seven-chapter campaign from Broken Bridge
type: feature
created: 2026-10-02
status: done
baseline_commit: 1e163ed14da3a95f664d40d25b3573e4ecefe788
context: [AGENTS.md]
---

<frozen-after-approval reason="User explicitly authorized completing the continuous campaign and extending to at least seven chapters">

## Intent

**Problem:** The visual demo replays one hard-coded map and does not connect battle results to persistent story progression. Closing the game can discard settlement, and rebuilding squads or names can undo losses.

**Approach:** Build one continuous campaign using the existing CampaignState, RefitCamp, Roster, CampaignLedger, BattleController and renderer. Deliver seven configured chapters, with the first three proving casualties carry forward, disk resume works, and earlier decisions gate a real noncombat resolution. Keep the current polished presentation and smooth camera.

## Boundaries & Constraints

**Always:** Preserve original branch history; work on codex/three-chapter-campaign in an isolated checkout. Use existing JsonValue, layer direction Engine ← Gameplay ← Campaign, original portraits and rendering. Persist campaign at chapter boundaries and after camp decisions; active combat resumes from its saved prebattle checkpoint, explicitly explained in UI. Settlement is exactly once. Dead named characters stay dead. Rescue and pursuit have meaningful mission/reward differences. New campaign is explicit; failed reads/writes retain the current campaign and previous save.

**Ask First:** Publishing further GitHub changes or replacing a user save without an explicit new-campaign action.

**Never:** Force push, reset iris, change third-party source, introduce network dependencies or frame-by-frame disk I/O. No claims that planned historical eras, AAA models, full governance or myth simulation are already implemented.

## I/O & Edge-Case Matrix

| Scenario | Input/state | Expected behavior | Error handling |
|---|---|---|---|
| New campaign | No save | Intro, initial army, first chapter briefing | Asset errors visible, no invented fallback chapter |
| Battle settlement | First resolution | Absorb actual casualties, merge named roster, save aftermath | Failure shows retry and prevents duplicate settlement |
| Resume | Valid briefing or aftermath save | Restore chapter, choices, army and results | Bad schema/types/IDs/values rejected without mutation |
| Peace gate | Accepted surrender in chapter 1 and successful rescue in chapter 2 | Chapter 3 negotiation bypasses battle, records noncombat completion | Otherwise show unmet conditions and battle option |
| Refit | Wounded veteran or recruits | Paid healing only; uniquely named recruits | No resurrection or reset of original army |
| Campaign end | Seventh chapter completed | Summary of decisions and losses, explicit completed state | No eighth chapter silently selected |

</frozen-after-approval>

## Code Map

- Campaign/CampaignState.h/.cpp and ChapterState.h: existing aggregate save facade; extend optional progress and make Windows replacement genuinely atomic.
- Campaign/ChapterLibrary.h/.cpp and CampaignFlow.h/.cpp: new cached chapter definitions, validated decisions, settlement and progression.
- Gameplay/Roster.h/.cpp and RefitCamp.cpp: safe deserialized records, persistent captain identity and unique recruitment.
- Gameplay/BattleController.h/.cpp: explicit mission resolution for timed protection/withdrawal.
- Examples/DuanqiaoPlayable.cpp: title, briefing/choices, live battles, aftermath and next chapter.
- assets/campaign and assets/maps: seven authored definitions and distinct battlefields.
- Examples/CampaignFlowTest.cpp and CMakeLists.txt: headless acceptance and existing regression targets.

## Tasks & Acceptance

**Execution:**
- [x] Campaign and assets -- implement validated data-driven seven-chapter flow and atomic progress persistence.
- [x] Gameplay/Roster and RefitCamp -- preserve casualty and captain continuity; reject invalid values and skip zero-strength deployment.
- [x] Examples/DuanqiaoPlayable.cpp and BattleController -- integrate new/continue, narrative briefing, mission choice, once-only aftermath, paid refit, seven battle configurations, conditional peaceful chapter 3 and final ledger.
- [x] Examples/CampaignFlowTest.cpp and CMakeLists.txt -- test complete seven-chapter flow, both chapter-3 routes, casualties across save/resume, corruption rejection and idempotent settlement.
- [x] Temp launch/build scripts and docs -- deliver one-command launch and clearly explain checkpoint resume and test results.

**Acceptance Criteria:**
- Given chapter 1 ends, when the player proceeds, then chapter 2 uses surviving units with their true remaining strength and prior wounded pool.
- Given a saved campaign, when the executable is restarted and Continue is selected, then chapter, story decisions, persistent names and supply balance are restored.
- Given mercy and a successful rescue, when chapter 3 negotiation is selected, then progression continues without combat or invented casualties/loot; otherwise negotiation is unavailable with a reason.
- Given any battle resolution or save retry, when the aftermath screen is revisited, then loot and casualties are applied once.
- Given all seven chapters, when completed through either allowed route, then a terminal summary is shown and no out-of-range chapter is loaded.
- Given the prior visual renderer, when campaign UI and battles run, then assets are loaded outside the frame loop, rendering remains VSync/MSAA enabled, and measured frame timings are reported without a universal FPS guarantee.

## Spec Change Log

## Design Notes

Chapter content follows the existing interrupted historian frame: Broken Bridge, annihilation ledger, first noncombat resolution, weeping shrine, coalition, betrayal, myth crossing. First four chapters belong to the warlord arc and the next three to the expedition arc. These seven are an authored playable segment, not a claim to finish the full fifteen-chapter narrative. Save phase and completed chapter IDs prevent replaying settlement after reload. Separate live battle roster from pointer-free campaign history. Early defeat remains a recorded setback; limited explicitly documented reinforcements keep progression possible without restoring dead veterans.

## Verification

Use a separate MSVC/Ninja build with existing cached GLFW/ImGui dependencies. Run CampaignFlowTest, CampaignStateTest, RefitCampTest, PostBattleTest, Roster coverage, BattleSceneTest, QuantumFogBattleTest and RenderPipelineTest. Exercise rendered campaign intro, aftermath, second briefing, third peace resolution and final summary using a deterministic verification mode and capture screenshots/frame timings. Check available MinGW version; report C++20 incompatibility if the installed compiler cannot build this project. Do not publish until separately requested.


## Verified Results

MSVC Release build and eleven acceptance/regression executables pass. CampaignFlowTest: 79 checks. Natural seven-chapter peace and combat routes both pass with disk reload at every boundary (21 and 23 permanent deaths respectively). Combat scene means 16.67–16.70 ms; p95 18.41–18.80 ms. Fourth through sixth chapters run at 960×640 for responsive UI verification. Installed MinGW GCC 6.3 cannot support C++20. No remote publication.

Review patches: retain off-field wounded under new officers with original class/template/relics; reserve capacity does not block full camps; immediate recruit roster registration; restore theme/scale settings; reject invalid template IDs; shared transactional checkpoint tested with failed settlement and once-only retry.

## Suggested Review Order

**Entry and progression**

- Connect persistent state to the game shell and battle lifecycle.
  [DuanqiaoPlayable.cpp:748](../../Examples/DuanqiaoPlayable.cpp#L748)

- Reject inconsistent progress and preserve peaceful route prerequisites.
  [CampaignFlow.cpp:53](../../Campaign/CampaignFlow.cpp#L53)


**Persistence and casualties**

- Commit to memory only after a complete validated disk checkpoint.
  [Checkpoint.h:17](../../Campaign/Checkpoint.h#L17)

- Replace the previous save without deleting it first.
  [CampaignState.cpp:25](../../Campaign/CampaignState.cpp#L25)

- Carry real losses and preserve off-field wounded without reviving officers.
  [RefitCamp.cpp:26](../../Gameplay/RefitCamp.cpp#L26)


**Presentation and evidence**

- Expose choices, refit, resume and a seven-chapter terminal ledger.
  [CampaignShell.h:35](../../Examples/CampaignShell.h#L35)

- Verify both routes, corruption, casualties and failed settlement retries.
  [CampaignFlowTest.cpp:62](../../Examples/CampaignFlowTest.cpp#L62)

- Verify real protection timer success, death/rout failure and withdrawal.
  [CampaignMissionTest.cpp:12](../../Examples/CampaignMissionTest.cpp#L12)

