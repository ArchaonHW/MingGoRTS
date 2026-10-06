# Spine Pair Review — MingGoRTS (coverage & internal-consistency lens)

- **DESIGN.md:** `ux-MingGoRTS-2026-09-18/DESIGN.md`
- **EXPERIENCE.md:** `ux-MingGoRTS-2026-09-18/EXPERIENCE.md`
- **Decision log:** `ux-MingGoRTS-2026-09-18/.decision-log.md`
- **Lens:** decision-log coverage, frontmatter/token integrity, cross-doc consistency, [ASSUMPTION] hygiene, surface completeness. Severity = downstream impact.

## Overall verdict

Structurally strong pair: canonical section order holds, all 17 `{path.to.token}` references resolve, and the four-theme token contract is coherent. Two integrity problems sit at the contract level — a fabricated `R6` provenance covering two unresolved log items, and a dropped GDD art decision (VRM commander portraits) — plus a systematic gap: only the battle HUD has real visual treatment; title/planning/pause/settings/post-battle are IA-only.

## Findings

### Critical

- **[critical] Fabricated `R6` citations mask unresolved decisions.** EXPERIENCE.md:83 attributes 急停 semantics ("strongest of the three candidate semantics, chosen at R6") and EXPERIENCE.md:146 attributes the myth→theme picker ("(decision R6)") to a decision round that does not exist — `.decision-log.md` runs R1–R5 then a Finalize entry, and Finalize (log:45) explicitly lists 急停語義 and 神話事件→主題映射 as **待裁 ASSUMPTIONs**. Downstream consumers will treat two still-open questions as ratified. *Fix:* re-tag both as `[ASSUMPTION]` or append a real R6 entry to the log.
- **[critical] Dropped decision: 具名指揮官 VRM 肖像 (log:14).** R2's GDD-inherited art direction includes named-commander VRM portraits; no spine surface anchors them — the dossier spec covers only aggression/discipline/cunning meters (DESIGN.md:330, EXPERIENCE.md:35,86). The VRM asset pipeline has no UI home; IA and Components both need a portrait slot. *Fix:* add a portrait element to the dossier component + IA entry, or log a deferral.

### Major

- **[major] Non-battle surfaces lack visual treatment.** IA covers all six shells, but DESIGN.md specifies visuals only for battle-HUD components: title screen (EXPERIENCE.md:21) has zero visual spec anywhere; pause (:36) and settings (:37) get only the "replace the view" stacking rule (DESIGN.md:287); the **planning war-room** (:22–26) — half the game — has no dedicated spec for the map table, deploy zones, objective/rally pins, Validate footer, or AI advisor panel; post-battle (:38) is covered only by the timeline component (DESIGN.md:323–324), leaving report/roster screens visually unspecified. *Fix:* per-surface visual treatment blocks, at minimum for title and planning.
- **[major] Missing `sources:` frontmatter in both spines.** Log:8 enumerates six input sources (gdd.md, epics.md, brief.md, addendum.md, roadmap, REFERENCE_GAMES_MECHANICS.md); neither DESIGN.md:1–178 nor EXPERIENCE.md:1–7 declares them, so inheritance/traceability is unverifiable per the validate rubric. *Fix:* add `sources:` list to both frontmatters.
- **[major] Myth-seepage semantics drift + internal contradiction.** R5 fixes seepage as swap "到對應主題" (event-mapped theme, log:37); EXPERIENCE.md:37/146 converts this into a player-picked 「神話層主題」 setting — a legitimate extension, but unlogged, and it collides with EXPERIENCE.md:145 which still says the UI "transitions to **the event's mapped theme**." The doc simultaneously claims event-mapping and player-picking. *Fix:* pick one model (player-pick with content-suggested variant, per :146, reads as the intent), align :145's wording, and log it.
- **[major] Stratagem menu reserved in DESIGN, absent from EXPERIENCE.** DESIGN.md:273 and :343 reserve a stratagem-menu slot beside the CP panel/doctrine strip; EXPERIENCE.md IA and Component Patterns never mention stratagems — a reserved surface with no behavioral spec or state model. *Fix:* add an IA stub + reserved primitive entry, or drop the reservation.

### Minor

- **[minor] `spacing.8 = '34px'` breaks the scale** (DESIGN.md:132). The numeric scale runs ×4 (1→4, 2→8, 3→12, 4→16, 6→24, 11→44); `8→34` fits neither the pattern nor the stated "8px base grid" (DESIGN.md:276). Likely typo for `32px`. *Fix:* correct or rename to a named token.
- **[minor] Three `colors:` tokens are `rgba()`, not hex** — spec requires hex strings: `theme.war-map.friendly-fill` (DESIGN.md:35), `theme.tactical-sim.panel` (:68), `theme.ink-chronicle.ink-faint` (:90). Also the ink-chronicle prose palette (:240) lists `ink-faint` with no value at all. *Fix:* supply hex + note alpha is applied at draw time, or document rgba as an accepted value form.
- **[minor] Stamina bar is an unmarked invention.** Appears in DESIGN.md:171 (roster cols), :308 (bar anatomy) and EXPERIENCE.md:46/48 (density tiers) with no basis in the log or sources and no `[ASSUMPTION]` tag — consistently propagated, which makes it look ratified. *Fix:* tag `[ASSUMPTION]` or source it.
- **[minor] No spines-win-on-conflict statement; weak mockup linkage.** The four files in `mockups/` are cited only inside YAML comments (DESIGN.md:24,43,65,85) and palette-section headers (:234–240); EXPERIENCE.md references a mock once (:104). Rubric expects each visual reference linked at the relevant section naming what it illustrates, plus a single precedence statement. *Fix:* add one "mockups are illustrative; spines win on conflict" line and per-mock captions.
- **[minor] 戰鬥長度 ~90s–4min dropped** (log:19). Battle-duration expectation never reaches the spines — it bounds the timescale options, feed volume, and replay-timeline density. *Fix:* note the duration envelope in EXPERIENCE.md Foundation or Battle state.
- **[minor] State coverage gaps.** State Patterns covers gameplay lifecycles well but no shell states: title 繼續戰役 with no save present, dossier at zero intel, empty feed, settings with unbound/conflicting keys (conflict detection is asserted at :126 but no conflict state is modeled). *Fix:* add empty/error rows where they apply.
- **[minor] "Three candidate semantics" has no source.** EXPERIENCE.md:83 references candidates for 急停 that the log never enumerates — unverifiable provenance even if the outcome is right. *Fix:* drop the phrase or record the candidates.

### Nit

- **[nit] 機率雲 semantics softened.** Log:14 says 閃爍候選簇 (flickering candidate cluster); both spines specify "slow turbulence drift" (DESIGN.md:167,318; EXPERIENCE.md:34,93) with no flicker term. Probably a deliberate readability choice, but it's a silent re-spec of a GDD art note.
- **[nit] Dotted YAML keys + dotted path syntax is resolver-ambiguous.** Keys like `color.morale-warn` referenced as `{colors.color.morale-warn}` resolve only under flat-key lookup; a naive nested resolver reads `colors→color→morale-warn` and fails. Works as written, but worth one line of resolution convention.
- **[nit] 格洛克's EN name never appears.** Log:18 fixes "Grok the Reckless" agr90/dis40/cun10; the spines use 格洛克 only, with the stat line buried in Flow 3 prose (EXPERIENCE.md:198) rather than the dossier spec where a consumer would look.
- **[nit] war-map 「← 士氣動搖!」 exclamation** (EXPERIENCE.md:66) is self-flagged as "the exception that proves the stamp rule" — acceptable, but it is still a second alert channel that the 達/急 stamp system was designed to replace; keep an eye on it.

## Mechanical notes

- **Section order (DESIGN.md):** Brand & Style → Colors → Typography → Layout & Spacing → Elevation & Depth → Shapes → Components → Do's and Don'ts — all 8 present, canonical order. ✓
- **Frontmatter:** YAML parses clean; all required spec keys present (`name`, `description`, `colors`, `typography`, `rounded`, `spacing`, `components`). Non-spec keys (`title`, `type`, `status`, `created`, `themes`) are harmless metadata. rgba() values noted above.
- **Token refs:** all 14 refs in DESIGN.md and 3 real refs in EXPERIENCE.md resolve to frontmatter tokens. `{path.to.token}` at EXPERIENCE.md:13 is notation documentation, not a reference.
- **Consistency confirmed:** theme names (war-map/command-room/tactical-sim/ink-chronicle), density names (極簡/標準/完整), anchor layout (DESIGN:268–274 ↔ EXPERIENCE:28–33), 急停 = 1 CP full-pause + hold-to-confirm (EXPERIENCE:83,105 ↔ log:45 pending), myth seepage revert/crossfade behavior (DESIGN:339 ↔ EXPERIENCE:98,148), CP buttons 攻進/撤回/駐守 keys 1/2/3 (DESIGN:303 ↔ EXPERIENCE:104), G-1/G-2/G-4/G-5/G-9 reservations, 1.1s collapse pulse cap (DESIGN:321 ↔ EXPERIENCE:157).
- **[ASSUMPTION] hygiene:** 11 tagged assumptions across the pair (myth hexes, menu labels ×3, outcome-ladder TC, timescale rates, camera scheme, bindings extent, myth-event specifics, EN +30% width) — all consistent with the log's 待裁 list except the two disguised as "R6" (critical finding above).
