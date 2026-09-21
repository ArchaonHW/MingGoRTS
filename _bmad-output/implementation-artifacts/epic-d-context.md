# Epic D Context: 神話雙層 (Myth Layer)

<!-- Compiled from planning artifacts. Edit freely. Regenerate with compile-epic-context if planning docs change. -->

## Goal

Implement the myth half of the History×Myth dual layer: a per-region seepage
state machine that escalates from quiet to open manifestation, interactable
shrine/spirit nodes on battle maps, the 天命 (myth-favor) currency that
converts into 民心/秩序, rare mid-battle myth incursions, and the
renderer-side seepage presentation. Myth-layer action is the primary
amplifier of the governance axis — pacifying shrines and laying ghost
legions translates directly into governance capital — and what is provoked
or pacified rewrites later battles. Serves pillars 2 (無戰而勝) and 4
(loss is content); covers FR10 and UX-DR5.

## Stories

- Story D.1: MythLayer 滲透狀態機 (seepage state machine)
- Story D.2: 神社與精靈實體 (shrine & spirit entities)
- Story D.3: 天命貨幣與轉換 (myth-favor currency & conversion)
- Story D.4: 神話入侵事件 (myth incursion events)
- Story D.5: 滲透視效消費端 (seepage visuals, UX-DR5)

## Requirements & Constraints

- Per-region seepage levels `0 quiet → 1 anomalies → 2 seep → 3 manifest`,
  driven by governance, slaughter density, and god-stance thresholds.
- Seepage never decreases within a chapter; manifest is chapter-scoped —
  the next chapter re-derives levels from persisted campaign state.
- 天命 accrues from pacified gods/spirits, honored dead, and laid ghost
  legions; conversion to 民心/秩序 requires favor ≥ threshold and deducts
  favor. Insufficient favor → the option greys out but stays visible
  (visible gate, never hidden).
- Incursions are rare mid-battle inserts reserved for meaning-changers:
  ghost-legion units appearing or false intel injected; the battle pauses
  briefly with a prompt. Pacified → governance credit; ignored → seepage
  advances. Trigger conditions come from chapter definition flags or
  seepage level.
- Shrine desecration feeds 墮落 — a one-way ratchet that never decays.
- Record-is-truth: every transition, interaction choice, conversion, and
  incursion emits a recorded event; incursions are logged with named
  provenance (which shrine, which spirit, when).
- All myth logic is headless-deterministic — pure transitions on numbers,
  testable without GL. No third-party JSON; versioned `potato.*` schemas
  with bool-return load that refuses bad input loudly.

## Technical Decisions

- `Campaign/MythLayer.*` owns seepage levels, transition conditions, the
  god-stance table, and the manifestation event queue — parallel in shape
  to `QuantumFog`. A battle-scoped view may live in `Gameplay/`; the
  renderer is a read-only consumer and never owns state.
- Dependency direction is `PotatoEngine ← Gameplay ← Campaign` only;
  `BattleController` must not know `Campaign` exists — myth/governance
  outcomes cross the boundary as recorder events/deltas aggregated by the
  `CampaignState` facade.
- Seepage and god-favor persist inside `potato.campaign/1` via the facade;
  sub-stores own `ToJson`/`FromJson` (schema-checked, bool-return).
- BattleMap data defines shrine nodes (position, god id, offering state) —
  an extension of the existing `potato.battle_map/1` map schema; content
  loads via the Library+LoadDir pattern, bad files skipped with warnings.
- Shrine verbs: pacify / provoke / sacrifice. Choices are recorded; results
  write into the GodStance per-god favor table (interface owned by Epic
  C.4). Shrine visual state (e.g. offering table tracking 民心) is exposed
  read-only for the renderer.
- Sound precedes visuals: level-2 transitions fire the myth soundscape
  before the visual change lands.

## UX & Interaction Patterns

- Myth seepage is the brand signature: a full-HUD theme swap signals which
  world layer is perceived — it rides a short transition motion, then ebbs
  back when the event ends. The readability interruption is deliberate.
- Presentation per level: 1 → local anomalies (fog tint, displaced
  shadows); 2 → myth soundscape + displaced objects; 3 → full theme swap
  (UITheme token swap + full-screen scrim transition).
- Anchors never move during swaps — CP, roster, and feed positions stay
  identical across themes; muscle memory is the contract.
- Critical info (morale-break, seizure countdown, reveal timers) stays
  dual-encoded and legible through the transition; never hue alone.
- Motion-reduction setting → transitions degrade to a crossfade. At most
  one full-screen scrim layer; the scrim crossfade is the required path,
  theatrical variants optional.
- Myth accents: `color.myth-gold` (#d4a23c) and `color.myth-foxfire`
  (#6fd8c8) for seepage frames, event icons, and replay-timeline ticks.
- Theme swaps are cheap: the token table drives `ApplyToStyle` at runtime;
  anchors are `SetNextWindowPos` constants.

## Cross-Story Dependencies

- **Epic B (landed):** `Campaign/` layer, `CampaignState` facade, and
  chapter-boundary saves must exist — MythLayer is a facade-aggregated
  sub-store; incursion flags ride `potato.chapter/1` chapter defs.
- **Epic C.4 GodStance:** D.2 shrine outcomes and D.3 天命 conversion
  read/write the per-god favor table — land the interface first.
- **MythLog (N-5):** D.4 incursions write named entries into the myth
  event log.
- **Epic A governance accumulators:** D.3 conversion and D.4 pacify/ignore
  outcomes target the 民心/秩序/墮落 sub-store.
- **Epic F.4 audio (parallel):** level-2 soundscape assumes the audio
  milestone; visuals must not block on it — degrade to silent.
- **Epic E (downstream):** 天命/god-favor thresholds defined here gate the
  無戰 council paths.
