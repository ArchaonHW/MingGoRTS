---
title: 'EXPERIENCE.md — MingGoRTS (民國 era doctrine RTS)'
type: ux-experience
status: final
created: 2026-09-18
updated: 2026-09-18
form_factor: 'PC keyboard + mouse (shipping); gamepad reserved via input-abstraction + radial-menu concept'
sources:
  - _bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md
  - _bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md
  - _bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/brief.md
  - _bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/addendum.md
  - _bmad-output/planning-artifacts/potato-engine-roadmap.md
  - docs/REFERENCE_GAMES_MECHANICS.md
---

# EXPERIENCE.md — MingGoRTS

## Foundation

PC title on PotatoEngine (C++20, OpenGL). UI is built on ImGui with a **runtime-swappable style driven by a token table** — the four themes in DESIGN.md are data, not code paths. PC keyboard + mouse ships; the input layer abstracts actions so a gamepad scheme (radial-menu concept) can bind later, but the HUD is **not** re-laid-out for handhelds (decision log R3). Bilingual 繁中/EN from day one via string table — no hardcoded strings. Visual reference for everything below: DESIGN.md tokens in `{path.to.token}` notation.

Player fantasy: a staff officer who wins the battle the night before. Two surfaces, one eye — the **planning war-room** (writing doctrine = issuing orders on a map) and the **execution battlefield** (camera in the mud, watching the orders collide). Emotional arc per battle: **command** (confident writing) → **suspense** (helpless watching) → **mourning** (named casualties in the roster). Hard UX gate: a non-programmer fields a battle in ≤ 10 minutes; doctrine editing must feel like arranging pieces, not coding.

## Information Architecture

Screen inventory (complete game shell — title → planning → battle → post-battle):

- **Title 標題** — 繼續戰役 · 新的戰役 · 設定 · 離開 [ASSUMPTION: menu item set; log fixes scope to "full shell" but not item labels]. Reserved: campaign/chapter-select hook (G-9 meta-layer refit lives one level below, between battles).
- **Planning war-room 規劃層** — the desk. Three columns:
  - *Left*: squad list (4 squads + 1 hero reserved frame) with deploy status; named captains show VRM portrait thumbs (initial-glyph fallback, see DESIGN.md §Officer portrait).
  - *Center*: map table — deploy zones, objective pin, rally pin (Alt+drag to place/move), reserved plan-arrow drawing layer (G-5 flagship: draw attack arrows → compile to doctrine).
  - *Right*: priority card slots (4 → 7 as the campaign grows) above the card editor — 8 triggers / 7 actions / threshold / priority / cooldown, arranged like pieces. Below: AI advisor panel (BattlePlanner) showing posture / role / rationale + 一鍵套用 one-click load.
  - *Footer*: Validate warnings (e.g. uncovered flank) + 開戰 commit.
- **Battle HUD 執行層** — always-on overlay, anchors per DESIGN.md §Layout:
  - Top center: phase (「交戰」), battle clock, timescale, pause.
  - Top right: intel `8/10`, CP pips `3/3`, 急停 emergency stop.
  - Left: squad roster — per-squad 兵力/士氣 bars, status tag, routing warnings, seizure stamps.
  - Right: doctrine event feed (chronicler voice).
  - Bottom right: CP intervention buttons 攻進/撤回/駐守 + **minimap** (new surface — missing from the current build; required: river/terrain strip, shape-coded dots — friendly square / enemy diamond / cloud blurred circle — camera frame).
  - Bottom left: doctrine card strip (read-only mirror; cards flash as they trigger — the score assembling itself) + reserved stratagem menu slot (one-shot CP stratagems, GDD secondary economy).
  - On-map: probability clouds (scale ∝ P, turbulence drift), reveal-timer rings, entanglement links between clouds, unit markers w/ class icons + counter hints (G-2), morale bars on selected units, rout-shock chain arcs (G-1), terrain layer toggle (G-4), order+seizure countdowns.
  - Contextual: enemy-general personality dossier (格洛克 aggression/discipline/cunning bars, unlocked progressively by intel spend).
- **Pause 暫停** — replaces the view (never stacks): 繼續 · 設定 · 放棄戰鬥 [ASSUMPTION: abandon-battle item; not enumerated in log].
- **Settings 設定** — 語言 (繁中/EN) · 基底主題 (4 themes) · 神話層主題 (player-picked myth seepage theme, default ink-chronicle) · HUD 密度 (極簡/標準/完整) · UI 縮放 75–150% · 字級 · 動態減弱 (shake + seepage motion off) · 神話滲透 on/off · 音效 · 按鍵 bindings overview.
- **Post-battle 戰後** — outcome ladder banner → chronicler report (typed character-by-character) → two-column roster (survivors | fallen+relics; enemy fled/executed) → replay timeline (scrub + event jump) → 再啟戰局 / 返回整備 [ASSUMPTION: button labels; the refit return anticipates G-9].

### HUD density tiers (player-adjustable)

| | 極簡 minimal | 標準 standard | 完整 full |
|---|---|---|---|
| Top strip | phase + clock + pause + **intel count** | + timescale | + dossier summary chip |
| CP / 急停 | always | always | always |
| Squad roster | status pips + alert badges only | name + 兵力/士氣 bars + status tag | + stamina bar + execution-rate % |
| Event feed | alerts only (急 entries) | full feed | + cooldown/execution detail lines |
| On-map | clouds + reveal rings + rout warnings | + selected-unit morale, counter hints on selection | + all-unit stamina, entanglement links, terrain layer, rout-chain arcs |
| Minimap | yes (collapsed smaller) | yes | yes + camera frame + pin markers |
| Doctrine strip | hidden | flash-on-trigger only | always visible |

Density rule from decision log R3: more than ~12 persistent items kills the fantasy — persistent chrome stays minimal and everything else expands on selection/focus, never on hover alone.

**Alert channel** (works at every density, overrides collapse): intel-expiring warnings (a reveal about to lapse pulses the cloud *and* emits a feed entry before fading), rout/collapse, CP-spent confirmations, myth-event onset. Spendable resources (intel, CP) are never invisible — the 極簡 tier still shows their counts in the top strip.

## Voice and Tone

**史官體 is the house voice.** The UI speaks as the chronicler recording the battle: terse, archival, past-facing. Rules:

- Name over number, always. 「老周 歿於斷橋」 not "Unit lost."
- Clipped military phrasing; a doctrine event reads like a dispatch line: 「前鋒營 觸發『接敵→交火』」「偵查隊 接觸揭露:敵·第3縱隊」.
- Alerts escalate by stamp, not by shouting: ordinary entries carry 達, urgent entries carry 急 — the word 急 does the work that "!!!" would do elsewhere.
- No second person, no system-speak. The game never says "your squad's morale triggered a threshold" — it records 「後衛營 士氣<30→撤退 執行率70%」.
- Mourning register: the roster lists the fallen by name with relics; this is where the emotion arc lands. No jokes, no gamified consolation.

**Ledger conventions** (from `_bmad-output/narrative-design.md` — the pen has its own grammar):

- **據報 hearsay marker** — myth-layer events enter the feed and the report as 「據報」 ("reported"), never as confirmed fact: 「據報 祠廟顯靈」. The ledger cannot afford to believe; even an event witnessed by thousands is still recorded as 據報.
- **判詞 rumor-state** — the dossier carries a one-line hearsay judgment of the enemy commander, systematically biased *low* (a fixed, learnable underestimate — scouting is fact-checking the pen). Intel spend verifies or falsifies it; verified truth replaces the rumor *visibly in place* — the correction is shown, never silent. High-cunning commanders may plant false判詞.
- **Omission count** — every after-action report closes with 「本報告省略 N 項」; the replay timeline is the audit tool for the gap. On the Fallen path the count balloons — the ledger protecting its writer.
- **Ledger leaks** — when the rival counters the player's habits, the report says so in his register: 「彼之陣法，似針對我軍慣用」.

**Per-theme register** (same voice, different desk):

- war-map — pencil marginalia: lowercase-energy annotations, dashed underlines, circled warnings (「← 士氣動搖!」 is the exception that proves the stamp rule — it's a crayon scream, not a system alert).
- command-room — telegram register: headers 軍電/急電, clipped phrases, stamps 達/急, clock-plate numerals.
- tactical-sim — bilingual terse: TC primary + `// EN` gloss, mono timestamps, `P=0.62` readouts.
- ink-chronicle — full chronicle register: 戰報 header, vertical titles, poetic counters (探·六息), vermilion miniseals on urgent lines.

**Outcome ladder naming** (five rungs): SubdueWithoutWar「不戰屈兵」· Victory「戰勝」· GovernedPeace「治平」· Defeat「敗北」· Fallen「覆滅」 [ASSUMPTION: TC renderings for the non-治平 rungs; EN keys are fixed].

**Bilingual behavior:** UI language is a settings toggle; both languages load from one string table. TC is the authorial voice — EN localizes *toward* the archive register, never toward casual game-speak. Layouts must absorb ~+30% EN width; no string concatenation of translated fragments.

## Component Patterns

Behavioral contracts only — visual spec lives in DESIGN.md §Components.

- **Squad roster row** — click selects (syncs on-map marker + opens unit focus); status tag carries state; alert state adds icon + accent edge ({colors.color.morale-warn}/`{colors.color.morale-break}`); seizure state overlays countdown stamp.
- **Card slot** — drag a card in, drag to reorder priority, click opens the editor; empty slot is a dashed outline 「空槽」. **Non-drag path (mandatory)**: a selected card + slot exposes ▲▼ priority steppers and a context action 「移至此槽」 — keyboard and gamepad never depend on drag precision. Fired cards flash in the execution strip as their motif joins the score.
- **Card editor** — trigger (8) → action (7) → threshold → priority → cooldown. Every field is a picker/slider/stepper — arranging pieces, never code. Valid combinations only; invalid pairs grey out with a one-line reason.
- **CP intervention button** — enabled when CP > 0 and a squad is selected; press starts the seizure countdown and spends 1 CP; the order stamps onto the squad when the countdown completes.
- **急停 emergency stop** — 1 CP buys a **full time pause**: the field freezes while the player can inspect, queue orders, and breathe; strongest of the three candidate semantics, chosen at R6. Always visible at every density; the pause state shows a distinctive 「急停」 overlay distinct from the free SPACE pause (which costs nothing). **Confirm affordance is configurable** — default hold-to-confirm (fat-finger guard); Settings offers double-tap or instant-press alternatives and adjustable hold duration (motor-impaired players must not be locked behind a hold).
- **Event feed entry** — click jumps the camera to the event location / selects the subject squad; urgent entries stamped 急.
- **Cloud marker** — click opens the intel context (Observe · 2情報 / Probe · 1情報); reveal progress shows as a sweeping ring + countdown numeral. **Hit-area floor**: cloud click targets never shrink below ~44px screen-equivalent regardless of probability scale — small-P clouds pad their hit area, not their visual (precision tax is not an accessibility feature).
- **Dossier panel** — VRM portrait header (initial-glyph fallback) + three labeled meters (aggression/discipline/cunning) filling row-by-row as intel is spent + a signature-habit line once unlocked + a **判詞 line** shown before any meter fills (hearsay, styled distinctly from verified data — see Voice and Tone). The face is part of the read: enemy commanders are meant to be recognized, not just measured.
- **Map pin** — Alt+drag places/moves objective and rally pins in planning; pins render on the battle minimap. **Non-drag path**: with a pin selected, arrow keys (or d-pad) nudge it cell-by-cell, Enter confirms — drag is the fast path, never the only path.
- **Timeline** — scrub handle + event ticks; click a tick to jump; feed and timeline share selection.
- **Radial menu** — reserved gamepad primitive: hold the context modifier → radial of the same actions LMB/RMB expose. Not shipped; the abstraction must not be designed out.

## State Patterns

- **Enemy contact (intel lifecycle):** `hidden` → `suspected` (probability cloud, scale ∝ P, drifts) → `revealing` (Observe/Probe committed, sweep ring + countdown) → `revealed` (confirmed enemy marker + dossier progress) → `expiring` (stale intel fades back toward cloud). Entangled clouds share reveal state and are drawn linked.
- **Morale:** `steady` → `wavering` (warn tag + annotation) → `break` (⚠ + broken-fill bar + routed din) → `routing` (moving, can trigger rout-shock chains G-1) → `rallied` or `destroyed` (roster casualty).
- **CP seizure:** `doctrine-auto` → `seizing` (countdown ring on squad + roster stamp, 「接管中 08s」; mix mutes into 離魂) → `seized` (manual order executes) → `released` (doctrine resumes) or `interrupted` (squad destroyed/routed).
- **Doctrine card:** `idle` → `armed` (in slot at commit) → `triggered` (feed entry + motif joins score) → `cooldown` → `ready`. Partial execution reports a rate (「執行率70%」).
- **Battle:** `planning` → `committed` (開戰 seals; pins and slots lock) → `execution` (phase machine: 「交戰」 etc.) → `resolution` (outcome ladder) → `post-battle` (report → roster → replay).
- **Theme:** `base` (player setting) ⇄ `seeped` (myth event: full swap in → hold → swap back). Player override and motion-reduction both collapse the transition to a fade.

## Interaction Primitives

- **LMB** — select squad/marker; click cloud → intel options (Observe/Probe); click feed entry → jump to context.
- **RMB** — context command on map/squad; probe action on clouds.
- **CP buttons** — 攻進 / 撤回 / 駐守, each CP-1, bound to keys 1/2/3 (per tactical-sim mock); apply to the selected squad.
- **急停** — 1 CP full time pause (hold-to-confirm); distinct from free SPACE pause.
- **Alt+drag** — place/move objective and rally pins (planning).
- **Drag** — card into slot; drag to reorder priority.
- **SPACE** — pause; timescale cycles ×1 → faster (chips display current rate) [ASSUMPTION: rate set not fixed in log].
- **Camera** — pan/zoom per RTS convention [ASSUMPTION: edge-pan/WASD/wheel-zoom unconfirmed in log].
- **Gamepad (reserved)** — every primitive above exists as an abstract action; radial menu surfaces context actions on a held modifier. No hover-only affordances exist to lose.

## HUD & Diegetic UI

The HUD is **semi-diegetic by construction**: it is not chrome *about* the battle, it is the physical desk the officer works at — paper slips, brass plates, log rows, brush strokes depending on theme. Two layers:

- **In-world (truth layer):** probability clouds, reveal rings, entanglement links, unit markers, morale bars on selected units, rout-chain arcs, terrain overlay, plan arrows (G-5). These live on the battlefield and are identical in meaning across themes.
- **Overlay (staff instruments):** roster, feed, CP panel, top strip, minimap, dossier. These are the officer's instruments — restyled per theme, repositioned never.

Rule: on-map elements are truth (positions, probabilities); overlay elements are instruments (resources, records). Myth seepage restyles the overlay *and* the rendering of the truth layer (clouds re-ink, markers re-stamp) while leaving positions untouched. During cinematic moments non-critical overlay fades; CP, 急停, and active alerts never hide.

## Input Schemes

- **Keyboard + mouse (ships)** — full RTS scheme; every action has a binding; LMB/RMB/Alt+drag/drag per Interaction Primitives.
- **Gamepad (reserved)** — same action set through the input abstraction; radial menu for context actions; glyph tokens in the string/icon table swap per detected device.
- **Full keyboard reachability** — every mouse action has a key path (accessibility floor); focus/selection reveals everything hover would. Drag-dependent actions specifically carry stepper/nudge alternatives (card priority ▲▼, pin arrow-key nudge — see Component Patterns); click targets carry a ~44px screen-equivalent floor.
- **Bindings screen** in Settings with conflict detection [ASSUMPTION: extent of remapping not fixed in log].

## Game Feel & Juice

Juice is how the fantasy lands — and it all respects the motion-reduction toggle. **Audio milestone gate**: the engine has no audio backend today (OpenAL commented out) — every audio cue below is an additive layer on top of visual signals, delivered when the audio milestone lands; nothing in this section is a blocking dependency for MVP juice.

- **開戰 commit** — the seal slams: wax/stamp/chop/border-flash per theme; the first doctrine motif arms into the score.
- **Doctrine fires** — feed entry stamps and *that card's motif joins the live score*. The player literally hears their plan execute; a collapsing flank sounds different from a clean one.
- **CP seizure** — press → the whole mix drops into the muffled 「離魂」 soundscape for the seizure's duration; countdown ring sweeps; the order stamps on completion. The world goes quiet when the officer reaches in.
- **Morale break** — audible routed-squad din; row pulses; broken-fill bar. No default screen shake (governed by motion toggle).
- **Intel countdown** — sweep ring + numeral (「偵查中 · 06」/「探·六息」); the reveal lands as the cloud condenses into a confirmed marker.
- **Cloud drift** — slow turbulence wander; scale ∝ probability. Under reduced motion: static bound + gentle pulse only.
- **Myth seepage** — the swap itself is the juice: a full-theme transition (ink bleed / lamp flicker / signal glitch as fits the target theme) on event start, and an ebb back on event end.
- **Chronicler report** — post-battle text types character-by-character over brush-and-paper audio; skippable, never auto-dismissed (mourning is the point).
- **Roster** — fallen names settle onto the page one line at a time; relics listed beneath.

## Theme System & Myth Seepage

- **Base theme** — player-selected in Settings: war-map / command-room / tactical-sim / ink-chronicle. Token-table swap at runtime; all four ship (assets batched).
- **Myth seepage** — a myth-layer event triggers a **full theme swap**: the whole UI transitions to the player's chosen myth theme (see next item), holds for the event, then swaps back to the base theme. Most dramatic option chosen deliberately (decision log R5); readability interruption is accepted and carried by the transition motion.
- **Myth→theme mapping is player-selectable** — Settings carries a 「神話層主題」 picker: the player chooses which theme the myth layer speaks in (decision R6). Default: `ink-chronicle` — the register closest to the supernatural. Content data may still tag events with a *suggested* variant (e.g. darkened ink for ghost-legion events), but the player's pick governs the base swap.
- **Transition spec** — bounded motion (swap-in ≈ one breath, hold, ebb-back); anchors never move; gameplay state (seizure countdowns, reveal rings, morale breaks) remains dual-encoded and legible throughout.
- **Revert & override** — seepage always reverts to the base theme at event end. Settings can disable seepage entirely (「神話滲透」toggle); motion-reduction forces a plain crossfade instead of the theatrical transition.
- **Fallback** — if a theme's shader/sprite dependencies are unavailable, swap degrades to token colors + rect geometry (the room changes, the furniture doesn't).

## Accessibility Floor

Commercial floor, all four pillars from the decision log:

- **Colorblind dual-encoding** — nothing communicates by hue alone: faction = marker shape + fill, morale = icon + broken-fill pattern, clouds = blob shape + `?`/P readout, alerts = tag + stamp.
- **UI scale 75–150%** + independent text-size adjustment; legibility floor — `faint`/gloss text never carries sole gameplay meaning.
- **Motion reduction** — complete per-effect fallback table; nothing keeps moving silently:
  | Effect | Reduced-motion fallback |
  |---|---|
  | Myth-seepage swap | plain crossfade |
  | Collapse pulse (1.1s, the fastest animation — photosensitivity-capped) | single static flash frame |
  | Reveal sweep ring | instant fade-in of countdown |
  | Seal slam (開戰/急停/接管) | stamp appears, brief opacity fade |
  | Chronicle typewriter | full text appears; brush audio only |
  | Cloud turbulence drift + gentle pulse | static bound, no pulse |
  | Screen shake (not default-on anyway) | off |
- **Full keyboard + gamepad reachability** — no hover-only information; every hover affordance has a selection/focus equivalent.
- **Bilingual TC/EN** — string table from day one; neither language is a second-class build.
- **Text contrast** — primary text ≥ 4.5:1 on its surface or scrim.

## Inspiration & Anti-patterns

| Source | What it lends | What to avoid |
|---|---|---|
| **Total War** | Camera-in-the-ranks execution layer; morale as readable battlefield weather | Hidden morale math; decoration-for-its-own-sake HUD |
| **CoH3 / SD2** | Suppression & rout readability; squads as the unit of empathy | Per-order APM tax; icon clutter around every unit |
| **SC2 / AoE4** | Information hierarchy, feed discipline, esports legibility (tactical-sim theme) | Optimization pressure that punishes *watching* — MingGoRTS is designed for suspense, not APM |
| **HOI4** | Plan-then-execute framing; the map table as the planning surface | Spreadsheet opacity; tooltip walls; plans the player can't read back after committing |

The signature no reference provides: the chronicler voice + myth seepage — the HUD itself reporting which layer of the world you're perceiving.

## Key Flows

### Flow 1 — First battle: 陳排長 learns regret (tutorial)

陳排長 is a new platoon officer; the tutorial teaches *regret*, not syntax — the syntax is already filled in.

1. Title → first chapter drops him straight into the planning war-room. His doctrine slots arrive **pre-filled** — four cards someone else wrote for him. The AI advisor panel offers its posture/rationale; he doesn't need it yet. Validate shows no warnings. He presses 開戰; the seal stamps.
2. Execution: the pre-filled plan works. Cards fire, the feed records 「前鋒營 觸發『接敵→交火』」, the score assembles — for the first half, he is winning and he did nothing.
3. Then the flank: an unguarded approach the pre-filled deck never covered. A cloud resolves into enemy markers where nothing should be; 後衛營's morale bar drains, the warn tag appears, the feed stamps 急.
4. He reaches for CP — but the collapse is scripted to outrun him. The 離魂 muffle swallows the mix; the seizure countdown isn't fast enough. 老周, a named captain, dies at the broken bridge. The routed din is the tutorial's real message.
5. Post-battle: the outcome banner, then the chronicler typing 「老周 歿於斷橋」 character by character, then the two-column roster opening on its own — survivors beside the fallen. Mourning is the lesson: the deck that "worked" killed a man he now knows by name.
6. The report names the enemy: 格洛克 — and the dossier shows what the intel already knew about his aggression. The loop is set: next time, write it himself.

### Flow 2 — Planning → execution intervention: the CP save

Returning to the desk, 陳排長 revises the plan himself.

1. Planning: he drags a 接敵→交火 card higher in priority, adjusts 士氣<30→撤退's threshold, places a rally pin (Alt+drag) covering the flank that killed 老周. Validate clears; 開戰 commits.
2. Mid-battle: the flank probe comes again — a cloud swells where he predicted. This time the feed records his card firing before contact; but 後衛營 still wavers: warn tag, morale bar into `{colors.color.morale-warn}` territory.
3. He selects the squad and hits 撤回 (key 2). CP pip spends, the mix mutes into 離魂, the seizure stamp counts down 「接管中 08s」 on the roster row and on-map.
4. The order lands before the break: 後衛營 falls back to his rally pin instead of routing. The din never starts. Doctrine resumes on release; the feed records the intervention as a stamped 急電.
5. Suspense resolved by reach — the fantasy of the officer who still has one hand on the table.

### Flow 3 — Post-battle revise loop: reading 格洛克

1. Post-battle report notes enemy behavior; the dossier panel has filled through the battle's intel spend: aggression 90, discipline 40, cunning 10 — 格洛克 attacks hard, holds loose, never feints.
2. 陳排長 scrubs the replay timeline to the flank-collapse tick and clicks it — the view jumps to the moment the cloud resolved. He watches where the attack *actually* came from.
3. Back in planning: he adds a contact-trigger card for the exposed approach and moves the deploy zone to cover it. The advisor's rationale line agrees with his read — or doesn't, and he learns who to trust.
4. Next battle: the cloud forms where he wrote for it; his card fires; the flank that killed 老周 holds. The roster at battle's end has no new names in the fallen column — and that absence is the game's reward.

### Flow 4 — Myth seepage (signature moment) [ASSUMPTION: specifics of myth events are content-defined]

1. Mid-battle, a myth-layer event fires — the HUD performs its full swap: paper becomes scroll, brass becomes ink, the whole room changes under the player's hands while the squad rows and countdowns stay exactly where muscle memory put them.
2. The event plays out in the seeped theme — clouds re-render as purple mist, the feed keeps recording in the same 史官體 voice.
3. Event ends; the theme ebbs back to the player's base. The battle continues — but the player felt the other world touch the desk.
