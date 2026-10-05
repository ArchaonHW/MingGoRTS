# Deferred Work

## Deferred from: code review of 1-1-gameplay-module-skeleton (2026-09-29)

- `static_assert` no-float enforcement on sim structs — nothing to assert until Story 1.3 (BattleState POD arrays); carry requirement into that story.
- `PotatoGameplay` does not link/exercise any engine leaf module — blocked on `Security/SecuritySystem.cpp` MinGW breakage (pre-existing). Revisit when engine leaf targets exist.
- `BuildEngine.bat` has no `-DPOTATO_BUILD_GUI=OFF` path — headless builds need manual cmake or the ASCII junction workaround (documented in AGENTS.md).
- `ENGINE_SOURCES`/`ENGINE_HEADERS` globs (pre-existing) lack `CONFIGURE_DEPENDS`; new `GAMEPLAY_*` globs get it, engine globs unchanged.

## Deferred from: code review of 1-2-jsonvalue-dom-parser-schema-guard (2026-09-29)

- Raw bytes ≥0x80 in strings not UTF-8-validated — accepted leniency (RFC 8259 permits); escaped `\u` paths are strictly validated. Add a validation pass if malformed UTF-8 ever breaks text rendering.
- `\u0000` embeds a raw NUL into `std::string` values — spec-legal; downstream `.c_str()` consumers would silently truncate. Reject only if a real consumer trips.
- `JsonValue` node is ~96 B (string+vector+map members per node) and `FindString`/`Items`/`Members` return refs that dangle after move/destroy — documented semantics; revisit only under measured memory pressure or a real bug.
- `potato_test_json` fixture files land in the implicit ctest working dir (build tree) — cleaned unconditionally on all paths; add explicit `WORKING_DIRECTORY` only if a second test ever races on the filenames.

## Deferred from: code review of 1-3-battlemap-model (2026-09-29)

- Unscoped bitmask enums (`Terrain`/`Strategic`/`HistMark`/`MythMark`) allow silent cross-domain mixing (`r.terrain & MYTH_SHRINE` compiles) — scoped-enums + flag-operator refactor deferred; fields are raw uint32 by design and `IsStrategicPoint`'s cross-domain bit test is intentional.
- `RegionIndexOf`/`FindRegion` are O(n) linear scans; the `indexOf` map built during validation is discarded — fine at tens-of-regions scale; retain the map only if measured hot.
- `IsStrategicPoint` hardcodes the capturable-bit mask — deliberate enumeration; revisit if a 4th `STRATEGIC_*` bit lands.
- Region/map `id` hygiene: whitespace-only, embedded NUL, invalid UTF-8 accepted — hand-authored content; strictness deliberately on structure, not content hygiene.

## Deferred from: code review of 1-4-squad-model (2026-09-29)

- `Squad` is not flat POD (`std::string id/name`) — when `BattleState::Checksum` lands (Story 1.6+/recorder), hash string *contents*, not object memory. Also gate: per-squad POD arrays vs struct-of-objects decision is made there.
- No rally transition (`Routing`→`Holding`) — deliberate design: rout is morale collapse, sticky in-battle; RefitCamp is the recovery path. Revisit only if a "rally" doctrine card is ever designed.
- `potato.squad/1` cannot express artillery's "(ranged)" qualifier or the counters/countered-by column — add `potato.squad/2` when combat resolution needs ranged/role data.
- Invalid `SquadState` values via public `state` field freeze the squad — accepted (serialization writes land in later stories; validate on deserialize then).
- `Heal`/`RestoreCohesion` no-op on terminal states; `ApplyHit` no-op on Routed/Destroyed — Routed squads are off-field (cannot be finished off); revisit if pursuit mechanics are designed.

## Deferred from: code review of 1-5-doctrine-interpreter (2026-09-29)

- `enemy_in_region`/`enemy_adjacent` triggers read ground-truth `regionIndex` — a truth-boundary seam. QuantumFog/CertaintyField (Story 1.8) must rewire these two kinds through certainty data; vocabulary stays stable, only the detector changes.
- `cpPool` is a single shared pool read by both sides — per-side CP pools land with symmetric AI (Story 1.10).
- `EvalTick` takes the snapshot as `const&` — "tick-start" purity is caller discipline, not enforced; if a future caller interleaves `ApplyDeltas` between squad evals this silently breaks. Snapshot-copy enforcement deferred until a real caller needs it.
- Determinism test is vacuous w.r.t. `rng` (vocab v0 draws nothing) — AC2 holds structurally; when a card draws PRNG, add a draw-order sensitivity test.
- `potato.doctrine_cards/1` is a library file vs architecture's singular `potato.doctrine_card/<ver>` naming — deliberate (consistent with potato.map/1, potato.squad/1); rename only if architecture text is updated.
- `ReadClause` treats `param` on a no-param kind (`always`, `hold`, `retreat`, `none`) strictly (only `0` allowed) — loosen to "ignore" if content authors find it hostile.

## Deferred from: code review of 1-6-battlecontroller-three-beat-loop (2026-09-29)

- Deployed squads share their template's `id` — two infantry from one template collide on battle-scoped identity. Events key on `squadIndex` so nothing corrupts; battle-scoped instance naming belongs to the roster/naming work (Epic B/C).
- `cpPool` has no upper cap (`SetCpPool` clamps negatives only) — CP economy (regen +1/60s, cap 5, intervention costs) is Story 1.7's job.
- Manual `RequestBeat(Aftermath)` mid-Execution is the concede path; `BattleOutcome::forced` distinguishes it from a wipe draw. Sub-story: whether "concede" should still produce a winner by objective score is Story 1.12 win-evaluation territory.
- `ROUT_TICKS` (2 s) is a v0 constant — rout-off-field pacing may want per-terrain or card-driven variance later.

## Deferred from: code review of 1-8-quantumfog (2026-09-30)

- No `potato.balance/1` file exists on disk — `FogConfig::FromJson` verified via inline JSON only; content loading wires in when the asset pipeline story lands.
- Truth boundary is discipline-enforced: `CheckGameplayDeps.ps1` bans Rendering/GUI includes but not `Gameplay/Squad` in `Gameplay/Fog`, nor doctrine truth-scans. Extend the guard when Campaign/Game layers appear.
- `BattleController::Squads()` exposes full enemy truth — fine pre-UI; presentation must consume `Fog(side)` for enemy rendering.
- `FogConfig` is a bare aggregate: out-of-range hand-built configs bypass `FromJson` validation (negative decayPerMinute drifts the accumulator).

## Deferred from: code review of 1-9-battleplan-arrows (2026-09-30)

- Replan `SimEvent` records only `param=path.size()` — the full path payload is lost when `pendingCommands_` clears at apply. Every other command is self-describing in the log; events-driven replay cannot reissue a Replan. Story 1.11 (BattleRecorder) needs command-input recording or a path side-channel anyway — resolve there.
- Replan on an arrowless squad creates and grants an arrow mid-Execution (deliberately kept — "replan = new plan" for CP). Re-check the contract when the symmetric AI opponent (1.10) starts issuing replans.
- No `potato.balance/1` file exists on disk — `PlanConfig::FromJson` verified via inline JSON only (same precedent as `FogConfig`); wires in with the asset pipeline.
- `PlanConfig` is a bare aggregate like `FogConfig` — hand-built configs bypass `FromJson` bounds (e.g. replanCost > CP_CAP).

## Deferred from: code review of 1-10-symmetric-ai-opponent (2026-09-30)

- Cloud-shaping is cunning-only: aggressive/defensive priors never touch fog. The AC mechanism is demonstrated on the prior that cares; a per-prior fog signature would be a design follow-up.
- `BattleAI::Act` reads `.side` on enemy squad rows to locate its own squads (short-circuit; no other enemy field touched). Mechanical own-index iteration lands when roster indices are tracked — same deferral bucket as `Squads()` truth exposure.
- `Plan` header claims "no mutation on failure" — held today only because all failure modes pre-validate; a future mid-loop `DeploySquad` gate would partially deploy. Latent.
- Cunning probes scan in region-index order, not weakest-certainty-first (comment says weakest); v0 acceptable.
- `BattleAI` ctor validates neither `side` nor `prior`; `Plan` gates bad sides and `Act` is benign (Issue* side-checks reject) — harden if AI objects are ever host-driven without Plan.
