---
baseline_commit: NO_VCS
---

# Story 10.5 — Document-Stream Integration

> Epic 10 — 本草綱目 Ben Cao Codex (BC) ·
> citation layer on existing render paths ·
> **Status: done**

## Story (from epics.md)

As a player,
I want the game's other voices to notice the book I'm reading —
the HistorianReport citing materia in passing
(「使役者服黃芪以固表」) and MythLog rumors preceding their
catalog verification (「用某藥者得自愈」),
So that the codex is woven into the chronicle, not bolted
beside it.

## Acceptance Criteria

- **Given** the unlocked set and a resolved chapter, **when**
  HistorianReport and MythLog render, **then** clause pools may
  cite only already-unlocked entries (a sealed entry is never
  named) and citations are deterministic for the same inputs
- **And** MythLog hearsay clauses for shrine/myth events
  reference entries the events will later unlock — rumor
  precedes catalog, never the reverse
- **And** no citation field alters report content semantics —
  they are color clauses riding existing render paths,
  removable without breaking the report.

## Context

Both voices exist and both are pure text folds:

- `RenderBattleReport(events, historianSide, seedLevels)` —
  史官體 (6.1). Citation = a 按語 colophon appended after the
  omitted-count line; it names ONLY entries in
  `codex.unlocked`.
- `RenderMythLog(log)` — 市井傳聞 (5.6). Hearsay = a free
  helper, one folk line per log entry, COMPOSED BY THE
  CALLER beside the log render — RenderMythLog's own
  signature is untouched (the strongest reading of
  "removable without breaking"). It names entries whose
  `myth_state` unlock matches the entry's `action` and which
  are NOT unlocked — the rumor precedes the catalog.
  Already-unlocked entries get no hearsay (the book has
  spoken; rumor is stale).

Direction rule, pinned:

| voice | names… | when |
|---|---|---|
| 史官 colophon | `unlocked` only | the book verifies what the field saw |
| 市井 hearsay | NOT-`unlocked` only | rumor precedes the page — never after |

A pending-補鈔 entry IS still nameable by hearsay — the codex
isn't verified until it lands in `unlocked`. The Scribe's
補鈔 queue is the rumor's destination, not its veto.

## Design

New leaf `Campaign/Narrative/BencaoCite.{h,cpp}` — two pure
helpers (both codex-first — the unlock state is the gate,
the library is reference data):

```cpp
// Colophon clauses for one battle — at most one citation per
// category, first unlocked entry in canonical Entries() order.
// Empty string when nothing qualifies (or either arg null).
std::string RenderBencaoColophon(
    std::span<const Gameplay::SimEvent> events,
    const BencaoCodex& codex, const BencaoLibrary& lib);

// Hearsay for ONE myth log entry — at most one line (first
// canonical match), folk register: FolkPlace-aware, seq-parity
// variant like the log's own FolkLine, names 釋名 aliases[0]
// (靈藥/異草 when aliasless — a sealed page's 正名 never
// leaks). Empty string when nothing qualifies.
std::string RenderBencaoHearsay(
    const MythLogEntry& e,
    const BencaoCodex& codex, const BencaoLibrary& lib);
```

`RenderBattleReport` gains two trailing defaulted params
(`const BencaoCodex* bcodex = nullptr, const BencaoLibrary*
blib = nullptr`); `RenderMythLog` is **not touched at all** —
the caller composes `RenderMythLog(log)` + per-entry
`RenderBencaoHearsay(e, …)` lines. Pass nothing → output
byte-identical to today (the "removable" requirement, pinned
by existing pins staying green).

Event-kind → category table (documented in file, derived from
the category bindings in Bencao.h):

| SimEvent kinds | 部類 |
|---|---|
| ConvoyArrived, ConvoyRaided | 蔓草/水草 manshui (河津漕運) |
| VillageOccupied, VillageBurned | 穀菜 gucai (村落糧秣) |
| ShrineCaptured | 金石 jinshi (祠宇金石) |
| InfiltrationChanged, MythActionInvoked, MythInvasion | 蟲獸 chongshou (靈異) |
| SquadExecuted | 毒草 ducao (暴行墮落) |
| everything else | — |

山草/隰草 are terrain-unlocked (the field doesn't carry
terrain flags — colophon stays silent rather than guess);
人部 is 10.6's late-beat content.

Colophon shape (史官體, elliptical register respected — no
numbers): one line per category present, category-ordinal
order:

```
書吏按：是役近河津，【名】之屬已錄冊中。
```

Per-category lead-in varies by category (河津/村落/祠宇/
靈異/暴行) — fixed strings, no PRNG.

Hearsay shape (folk register, one line per matching entry):

```
傳聞：里人言【名】之效，惟未見諸冊。
```

## Dev Notes — guardrails

- Names come from `entry.name` (正名), never aliases — the
  rumor and the colophon both cite the canonical title.
- Colophon cites at most ONE entry per category — the first
  unlocked in canonical order; a category with no unlocked
  entry contributes nothing (sealed pages are never named).
- Hearsay matches `unlockKind == MythState &&
  unlockParam == e.action` exactly; "infiltrated"-style
  non-action states never hearsay (no log entry emits them).
- No new includes in the renderers' headers beyond forward
  decls — BencaoLibrary/BencaoCodex are incomplete types in
  the signatures; the .cpps include BencaoCite.h.
- Deterministic: canonical order only; seq parity untouched;
  no PRNG, no clock, no I/O.
- Codex content never reaches the sim — this stays in
  Campaign/Narrative like its siblings.

## Implementation Tasks

- [x] `BencaoCite.{h,cpp}` — colophon + hearsay helpers (both
      free functions, both codex-first)
- [x] `BattleReport.{h,cpp}` — trailing codex/lib params,
      colophon appended post-confession line
- [x] `MythLog.{h,cpp}` — **untouched**; hearsay composes at
      the call site (see notes)
- [x] `potato_test_bencao.cpp` — 10.5 section (19 pins)
- [x] Build + ctest; story + sprint-status sync

## Dev Agent Record

### Completion Notes List

- Mixed-attribution landing (three design iterations across
  parallel sessions): final shape = colophon rides
  `RenderBattleReport`'s trailing params; hearsay is a free
  `RenderBencaoHearsay(MythLogEntry, codex, lib)` composed
  by the caller. RenderMythLog's signature is untouched —
  the strongest reading of the AC's "removable without
  breaking" (zero surface change to the 5.6 renderer; the
  log render and its pins cannot be perturbed by the
  citation layer).
- Hearsay is folk-register through and through:
  FolkPlace-aware (第N里/某處), seq-parity variant
  (據說/或云, deterministic — no PRNG), first canonical
  match only, names 釋名 aliases[0] with 靈藥/異草 fallback —
  a sealed page's 正名 never leaks, same 諱 convention the
  codex itself keeps. Colophon = canonical 正名 (the book's
  own voice).
- Param order normalized to codex-first on both helpers and
  the report renderer — the unlock state is the gate, the
  library is reference data.
- Event-kind → 部類 table as spec'd; 山草/隰草/人部 stay
  silent (no terrain visibility from the event stream).
- Direction rule verified by pins: colophon cites
  unlocked-only (locked jinshi never named, then cites after
  unlock); hearsay names pending/not-yet-unlocked and goes
  silent once the page lands in `unlocked`.
- Removability pinned: BattleReport null-arg calls
  byte-identical to pre-10.5 output; RenderMythLog needs no
  null-arg pin because its signature never changed — the
  compose-pin proves hearsay lines sit beside, not inside,
  the log render.

### File List

- `Campaign/Narrative/BencaoCite.{h,cpp}` — colophon +
  hearsay helpers (NEW)
- `Campaign/Narrative/BattleReport.{h,cpp}` — trailing
  `(bcodex, blib)` params, colophon call
- `Campaign/Myth/MythLog.{h,cpp}` — **unchanged** (hearsay
  composes caller-side)
- `Examples/potato_test_bencao.cpp` — 10.5 section
- `_bmad-output/implementation-artifacts/10-5-*.md`,
  `sprint-status.yaml`

## Validation

- `potato_test_bencao` green: 10.5 block — 2-category
  colophon, category-ordinal order, silent sealed categories,
  determinism, unaided byte-identical, locked-then-unlocked
  jinshi flip, helper seam equality, folk-alias hearsay,
  sealed-name non-leak, catalogued-silence, aliasless vague
  hearsay, unmatched action, empty log.
- `potato_test_myth` / `potato_test_campaign` /
  `potato_test_ledger` regression green.
- `ctest` **25/25** incl. `gameplay_dep_guard` (25 = the
  parallel 6.5 NarrativePack test joined the suite).
