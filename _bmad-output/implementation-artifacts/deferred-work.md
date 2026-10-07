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
- ~~Manual `RequestBeat(Aftermath)` mid-Execution is the concede path; `BattleOutcome::forced` distinguishes it from a wipe draw. Sub-story: whether "concede" should still produce a winner by objective score is Story 1.12 win-evaluation territory.~~ **Resolved in 1.12**: `CloseReason{Wipe,Concede,Stalemate}` distinguishes all three close modes; a conceded battle reports the field truthfully — `winnerSide` reflects effective counts even on concede (battle reports; campaign decides).
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

## Deferred from: code review of 1-12-win-evaluation (2026-10-02)

- `Squad::ApplyHit` has no caller outside `potato_test_squad` — no doctrine action deals damage in E0, so `Destroyed`/`LedgerEvent::Casualty` postings are structurally emitted but unexercisable through the battle API. Lands when a combat/damage action arrives (doctrine vocab or Epic 4 governance-field events).
- `potato.battle_record/1` gained `closeReason`/`stalemate` without a version bump — records are pre-release (generated+verified same-session; none persisted); bump the schema when durable records ship.
- `EventFromJson` `aux` bound 0..5 is kind-generic (BeatChanged 0-2, Intervention 0-5, CloseReason 0-2) — permissive by design; the replay diff is the semantic gate. Tighten per-kind only if a hostile-record DoS path ever materializes.

## Deferred from: code review of 2-1-five-account-double-entry-ledger (2026-10-03)

- `potato.ledger/1` has no hash fields — adding the chain (Story 2.2) should bump to `potato.ledger/2`, since a /1 loader silently skips chain verification and trailing-entry truncation is undetectable. Unknown per-entry fields are also dropped on resave — relevant when 2.3 adds the `suspect` flag. **(Resolved in 2.2 — `/2` shipped; truncation bound via `seal = Hash(count || tip)`.)**
- **2.3 design decision (recorded in 2.2 review):** the entry hash covers exactly `{seq, credit, debit, memo, tags}` — unknown fields are hash-transparent AND dropped on resave. If `suspect` lands as a plain extra field, an attacker can strip/flip flags without breaking the chain. If suspect flags need tamper-evidence they must enter `EntryContentJson` → `potato.ledger/3`; if provenance is deliberately NOT hash-covered (a forged entry's flag is external judgment, not part of the entry's claimed content) keep it outside and accept that the flag itself isn't tamper-evident — but then it must not be silently dropped on resave either. Decide deliberately, don't bolt on. **(Resolved in 2.3: provenance enters the hash as entry content — the forged mark is tamper-evident; suspect stays outside the hash as freely-mutable judgment metadata, but is a required field on resave. Schema -> /3.)**
- Dep guard is a textual tripwire: `-like` substring matching over-matches dir names ending `game/`/`gui/`/`rendering/` (none exist today), `#if 0` blocks still flag, macro-aliased `#include MACRO` evades it, and `.inl/.cc/.cxx/.ixx` extensions are unscanned. Sufficient as a cheap guard; upgrade to a real include-graph tool only if evasion matters.
- `Ledger` is copyable — copies diverge intentionally (save snapshots). `Entries()` references invalidate on Post (documented on the accessor).
- Engine `install(DIRECTORY …)` headers skip `Gameplay/` and `Campaign/` — pre-existing gap, flagged for completeness.

## Deferred from: code review of 3-4-chapter-shell-progression (2026-10-03)

- `ChapterState` — architecture lists a per-chapter runtime object under `Campaign/State`, but no Epic 3 story creates one. Mid-chapter data with no home today: the presented record-root set for `OrphanAnchors` reverse scans, fog priors carried into a chapter, per-chapter lock/phase state (a phase state would also close the resolve-spam hole — progression currently lets a caller resolve chapters back-to-back without playing them). Land with Epic 6 (intel/fog priors) or when the first per-chapter runtime field appears.

## Deferred from: code review of 3-5-persistent-roster (2026-10-03)

- `scars` as a distinct roster field has no producer or consumer yet — veterancy/casualties/dead are the persisting numbers the narrative layer reads. Add when RefitCamp wounds or governance produce them (schema bump).
- Dead entries are permanent memorials: corpses hold roster slots and names are never reused (no disband path). Long campaigns can exhaust `MAX_ROSTER` — intended weight of permanent loss; revisit only if recruitment pressure demands it.
- `GetRoster()` hands out a mutable `vector&` — the legit write paths (`Enlist`/`ApplyAftermath`) enforce wire invariants, but direct mutation can bypass them. `ToJson` re-validates the emitted roster so the seam can't produce an unloadable save; a const-only accessor remains the stronger fix if the seam ever bites.

## Deferred from: code review of 3-6-refitcamp (2026-10-03)

- `Plunder` is intentionally ungated — the -`民心` leg IS the cost. But nothing rate-limits it per chapter/visit; a per-visit plunder allowance belongs to the chapter shell's phase state (`ChapterState`, deferred from 3.4).
- `Balance(Materiel)` is spendable-truth: forged entries fund refits identically to honest income (documented 2.3 contract — suspicion is data, not exclusion). If a 'clean funds only' refit rule is ever wanted it needs a provenance-filtered balance fold.

## Deferred from: code review of 3-7-rivaldeck-learning (2026-10-03)

- `RivalBook` is a standalone `potato.rivals/1` doc — not yet embedded in `potato.campaign/1` nor wired into a save slot. Decide embed-vs-sibling-file when the save format next bumps.
- No production call site: the CardFired->trigger-histogram fold (reading the recorded battle to feed `RecordChapter`) and counter-deck->SquadSheet injection belong to the chapter-shell integration seam (`ChapterState`/Epic E). The AC is demonstrated as API+test.

## Deferred from: code review of 4-1-governancefield-battle-events (2026-10-04)

- A convoy still in flight when the battle closes emits no terminal event — silence = unsettled; the ledger fold (4.3) and chapter shell decide whether an unresolved convoy counts for anything. Also: convoys don't hold a battle open (wipe detection counts squads only) — deliberate for now.
- `ConvoyArrived.squadIndex` records destination-presence, not route escort; if accounting needs per-leg escort credit, add an escorted-legs counter to `Convoy` and carry it on the event.

## Deferred from: code review of 4-2-atrocity-auto-detection (2026-10-05)

- `BookDeeds` has no production call site — the detection/tagging seam is unit-tested but nothing invokes it at battle close yet. The aftermath wiring (recorded event stream + `BattleResult::ledger` -> postings + roster aftermath) belongs to the battle->campaign resolution pass: Story 4.5 (defeat conversion) or the Game shell chapter resolution, whichever lands the caller first.
- `Squad::ApplyHit` still has no production caller — `IssueExecute` kills via `hp = 0` + `ApplyEvent(HpZero)` (an execution isn't a hit roll). Damage mechanics remain deferred to the combat vocabulary.
- No doctrine-level "no quarter" stance — Execute is a per-victim intervention verb; an authored standing policy (e.g. a trigger/action pair that refuses all routs in a region) is Epic 7 vocabulary territory.
- Refuser presence accepts any Holding squad — a contested region does not shield a routing victim (contrast `ExclusiveSide` for village dwell). Deliberate: atrocity needs killers in reach, not control of the ground.
- `AuditSegment::atrocities` counts forged entries tagged "atrocity" too — the forgery channel can inflate the headline number; `audit.forged` exposes the source for cross-reference (suspicion is data, not exclusion — 2.3 contract).

## Deferred from: code review of 4-5-defeat-conversion (2026-10-05)

- Every ResolveAftermath call spends the chapter — single-battle chapters only. A mid-chapter non-sealing variant (ApplyAftermath + BookDeeds without ConcludeChapter) or an explicit chapterFinal flag belongs to whichever story introduces multi-battle chapters.
- `record_root:` dedup is a linear tag scan per settlement — fine at ledger scale today; if settlement frequency ever matters, index anchors.

## Deferred from: code review of 5-1-myth-infiltration-state-machine (2026-10-05)

- `ApplyMythEvent`/`SeedInfiltration` are Planning-phase verbs only — mid-Execution myth drivers (myth actions: pacify shrine, invoke possession, ghost armies) belong to Story 5.4, which owns its own journal mechanism (pending command or doctrine action).
- Journal-contract asymmetry: `myth`/`mythseed` ops now reject no-ops (accepted call = observable state change), but the older `cp`/`intel`/`sheet`/`convoy` ops can still be forged as state no-ops into a root-consistent record — pre-existing class, would need per-op change-detection to close.
- Verifier doesn't cross-check `toolVersion` against max-declared SimEvent kind — a v1-stamped record with kind-9 events verifies if self-consistent (fabrication is equivalent; note only).
- `BattleMap::MAX_REGIONS` (1024) was introduced for the myth cap + verifier resource bound; `MythState::MAX_REGIONS_PER_CHAPTER` aliases it — keep in lockstep.

## Deferred from: Epic 10 (Bencao Codex) planning (2026-10-05)

- `RosterEntry` has no `scars` field (deferred from 3.5) — the 續斷/骨碎補 unlock triggers in `bencao-worldview.md` §4 key on veterancy/casualties thresholds until scars land via a roster schema bump.
- `ChapterDef` has no terrain field — terrain-kind unlock triggers must read the `map` ref's `potato.map/1` terrain flags at chapter settle, or `potato.chapter/2` adds an explicit `codexTerrain` hint. Decide in Story 10.2. **(Resolved in 10.2: `TerrainFlagsOf(BattleMap)` reads the map's `TERRAIN_*` flags; the caller loads `ChapterDef.map` at settle time — no `potato.chapter` bump needed.)**
- Codex persistence: embed the unlocked set + pending 補鈔 queue in `potato.campaign` (schema bump) vs. sibling `potato.bencao_state/1` — same open seam as RivalBook (deferred from 3.7). Decide in Story 10.2; AC requires lossless round-trip either way. **(Resolved in 10.2: sibling `potato.bencao_state/1` shipped — `potato.campaign` untouched.)**
- `ResolveBencaoUnlocks` has no production call site (10.2) — the chapter shell (or Game-layer settle pass) must assemble `CodexSignals`, including loading the chapter map for `TerrainFlagsOf`. Same deferral class as `BookDeeds`/`RivalBook` (deferred from 3.7, 4.2).
- `DeliverBuchao` has no production call site (10.3) — same deferral class as above; the document-stream caller lands with the Game shell.
- Marginalia plumbing shipped scoped-to-codex (10.3): `BuchaoPage::suspect` flag + `BuchaoStore` — Story 6.6's AC says 10.3 "reuses" its plumbing but 6.6 was still backlog; the minimal store was built inside 10.3 (decided 2026-10-05). 6.6 may later generalize/unify; bencao pages keep their own store.
- OQ-B2 (poisoned/forged bencao page) — Story 10.3 reserves the suspect-flag plumbing but ships no poisoned entry; decision pending, see `bencao-worldview.md` §9.
- OQ-B3 (本草體 English rendering) — `potato.bencao/1` carries a `lang` block with zh-TW required; `en` deferred.
- OQ-B5 (人部 entry count 0/1/3) — Story 10.6 gates human-derived entries on this decision.

## Deferred from: code review of 10-2-bencao-unlock-engine (2026-10-05)

- `MAX_ENTRIES` overflow in `BencaoLibrary::Load` has no test fixture — writing 1025 fixture files is too heavy for the current Check style; the failure mode is a loud wholesale rejection, not silent corruption.
- `BencaoLibrary::Load` has no bound on candidate-file count or `rejected` vector size — systemic pattern shared with `ChapterLibrary::Load`; a hostile content dir could grow both unboundedly (each .json costs up to the shared 64 MiB read/parse budget). Loader-class hardening, not introduced by Epic 10.
- `CodexSignals::roster` is dead API until a veterancy/casualty-keyed unlock kind lands (scars deferral); DeedBook deed-kind reads are likewise unexpressed in the six kinds; the settled-chapter identity for `CodexSignals` comes from the settle caller, which today is unbuilt — all extend the existing wiring-seam deferral.
- `BencaoCodex::FromJson` doesn't reconcile persisted ids against the loaded library — ids for content entries that no longer exist silently persist. A `Reconcile(lib)` prune/flag is a design choice for 10.3+.

## Deferred from: Story 11.6 NFT collectible minting (2026-10-07)

- `PotatoToken.burnFrom` is `onlyOwner` — the relayer wallet is the trusted burner on the dev chain. Production would use standard ERC-20 allowance (`approve`/`burnFrom`) so the player authorizes the spend; dev-chain simplification recorded, not built.
- `PotatoCollectible` stores descriptors on-chain (dev chain: gas free). Mainnet would tokenURI to IPFS/Arweave and store only the hash — noted, not built.
- `frontispiece`/`dossier` gates are evidence-text placeholders until Epics 8/6 land real artifact state docs — then upgrade to schema-typed membership checks like `bencao`.
- `struck[key]` dedupe returns `false` (idempotent no-op, not revert) — if the UI wants "already struck" as a distinct signal, surface via `totalMinted` diff or an event; the contract emits no `Struck` event today.
- `collectiblePrice` is global (one price for all kinds) — per-kind pricing is a config-schema bump if wanted.
