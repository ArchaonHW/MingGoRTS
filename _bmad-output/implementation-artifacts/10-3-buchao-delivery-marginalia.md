---
baseline_commit: NO_VCS
---

# Story 10.3 — 補鈔 Delivery & Marginalia Binding

> Epic 10 — Bencao Codex (BC) · `potato.buchao/1` ·
> **Status: done**

## Story (from epics.md)

As a player,
I want unlocked entries to arrive as 「補鈔」 pages in the post-battle
document stream, each later bound with a Scribe 批註 tying the herb
to the event that unlocked it,
So that reference becomes memory.

## Acceptance Criteria

- **Given** an unlock set at chapter close, **when** the document
  stream renders, **then** new entries appear as 補鈔 pages
  (bounded per chapter, remainder queued) and each 批註 composes
  from the triggering event's context via clause pools **and** the
  marginalia path carries an authenticity-flag — a forged-looking
  批註 can be flagged suspect **and** 批註 never asserts efficacy
  beyond the 【主治】 field it annotates.

## Context

10.2 landed the unlock engine: `ResolveBencaoUnlocks` enqueues
triggered ids into `BencaoCodex`'s pending 補鈔 queue, and
`TakePending(max)` is the delivery seam — pending∩unlocked are
disjoint (a page the book doesn't hold can't be cited). This story
turns the drain into *pages*: the 補鈔 document plus the Scribe's
批註 bound to it.

**Dependency deviation (decided 2026-10-05):** epics.md says
"reuses Story 6.6's authenticity-flag plumbing" — 6.6 does not
exist yet. This story ships the **minimal plumbing scoped to the
codex**: a `suspect` flag on each delivered page (same judgment
overlay semantics as `LedgerEntry::suspect` — post-hoc, hash-exempt,
persisted). Story 6.6 remains for the ledger-wide marginalia
system and may later unify; recorded in deferred-work.

**Trigger provenance (landed 2026-10-06, review decision):** the
pending queue carries `PendingPage{id, kind, detail}` — `kind` is
the UnlockKind wire id and `detail` the matched evidence (tag
posted / action logged / flag found / corruption level / closing
chapter). The 批註 composes from that provenance — `%P`
substitutes `detail`, the clause pool keys on the recorded `kind`
(entry manifest kind as fallback).

**批註 efficacy rule** (AC literal): annotation text may quote or
gesture at the entry's `indications` field but must never fabricate
efficacy claims — clause templates cite, they do not invent.

## Design

`Campaign/Narrative/Buchao.{h,cpp}` — delivery pass + marginalia
store. Narrative layer (content systems live here); reads the
library and codex, writes only the buchao store + codex's unlocked
set (via TakePending). Zero sim involvement.

### Wire shape — `potato.buchao/1`

```json
{"schema":"potato.buchao/1",
 "pages":[
   {"entry":"sanqi","note":"是歲，軍中多傷，書吏錄此卷。",
    "suspect":false}]}
```

One document per campaign (sibling doc — same seam as
`potato.bencao_state/1` and `potato.myth/1`; `potato.campaign`
untouched). `pages` is append-ordered (delivery order); each entry
appears at most once (an entry can only leave pending once).

### Model

```cpp
struct BuchaoPage {
    std::string entry;     // bencao entry id — the page it annotates
    std::string note;      // 批註 text — composed at delivery
    bool suspect = false;  // judgment overlay — persisted, not hashed
};

class BuchaoStore {
public:
    static constexpr std::string_view SCHEMA = "potato.buchao/1";
    static constexpr std::size_t MAX_PAGES = BencaoLibrary::MAX_ENTRIES;
    static constexpr std::size_t MAX_NOTE_LEN = 512;

    const std::vector<BuchaoPage>& Pages() const;
    const BuchaoPage* Find(std::string_view entryId) const;
    bool SetSuspect(std::string_view entryId, bool suspect = true);

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    // Rejects bad schema/shape; dedupes by entry (first wins);
    // pages for never-delivered entries are tolerated on load
    // (judgment metadata, same surface as ledger suspect flags).
    static Gameplay::Result<BuchaoStore> FromJson(
        const Gameplay::JsonValue& doc);
};

// The delivery pass. Drains up to `maxPages` from codex's pending
// queue (TakePending — the drain itself writes them into the book),
// composes a 批註 per entry via clause pools keyed on
// `entry.unlockKind`, and appends the pages to `store`.
// `chapterIndex` salts the clause pick (see below). Returns the
// delivered pages in queue order. `maxPages == 0` delivers nothing.
Gameplay::Result<std::vector<BuchaoPage>>
DeliverBuchao(const BencaoLibrary& lib, BencaoCodex& codex,
              BuchaoStore& store, std::size_t maxPages,
              std::int64_t chapterIndex);
```

### Clause pools

Per-`UnlockKind` template arrays, code-resident like BattleReport's
史官體 clauses (content authoring at scale is 10.6's job). Pick is
deterministic: `FNV-1a(entry.id ‖ page.detail ‖ chapterIndex) %
pool.size()` — the same unlock always composes the same note
(replay-stable, no PRNG, no clock). Templates substitute only
`%N` (entry `name`) and `%P` (provenance `detail`), and may
gesture at `indications` but never assert beyond it (efficacy
rule above).

Six pools, one per kind — example shapes (final wording is
implementation latitude, register is 史官體):
- `terrain` — 「{name}，{param}之地所產，是役親履其土。」
- `ledger_tag` — 「{name}，因{param}一記入冊，書吏錄之。」
- `governance` — 「{name}，{param}之治既成，民間獻其方。」
- `myth_state` — 「{name}，聞{param}之異，土人指以為證。」
- `corruption` — 「{name}，兵燹既深，惡疾隨之，姑錄以戒。」
- `chapter_close` — 「{name}，本回既畢，照例補鈔。」

### Rules

- **Bounded per chapter**: `maxPages` drains FIFO; remainder stays
  pending for the next settle — the AC's queueing is literal.
- **Idempotent**: an entry already in `store` or already unlocked
  can't be re-delivered (pending∩unlocked disjoint + store dedupe).
- **Determinism**: clause pick is a pure function of
  (entry.id, chapterIndex); no clock, no PRNG.
- **Suspect is judgment**: `SetSuspect` flips a persisted flag —
  outside any hashed content, same surface as `LedgerEntry::suspect`.
- **No production call site** (same deferral class as
  `ResolveBencaoUnlocks`/`BookDeeds`): the chapter-settle caller
  lands with the Game shell's document stream.

## Implementation Tasks

- [ ] `Campaign/Narrative/Buchao.{h,cpp}` — `BuchaoPage`,
      `BuchaoStore` (To/FromJson, SetSuspect), `DeliverBuchao`,
      six clause pools + deterministic pick
- [ ] `potato_test_bencao` — extend with a 10.3 block (one test
      binary per content area)
- [ ] Review (three passes), sprint-status sync

## Dev Notes — guardrails

- Everything per 10.1/10.2 Dev Notes: layering (`Campaign/` →
  `Gameplay/` public headers only; **no PotatoEngine link**),
  `Result<T>` at boundaries, no third-party JSON (`JsonValue`
  only), PascalCase/camelCase/UPPER_SNAKE naming, MinGW junction
  build (`C:\MingGoRTS`), `uv`/`python`/`git` not on PATH.
- `DeliverBuchao` reads `lib.Find(id)` for each drained id — a
  drained id MUST resolve in the library or the delivery fails
  (`field`): a pending id with no entry is corrupt state, not a
  skippable row.
- The `suspect` flag persists on the page record — `potato.buchao/1`
  carries it (it is NOT hash-covered; there is no chain here).
- UTF-8 leniency inherited (JsonValue doesn't validate ≥0x80 bytes);
  批註 text is CJK by design — no validation pass added.
- Bounds: `entry` ≤ `MAX_ID_LEN`, `note` ≤ `MAX_NOTE_LEN`, pages ≤
  `MAX_PAGES` — file-is-untrusted.

## Validation

- `potato_test_bencao` 10.3 block pins: delivery drains ≤ maxPages
  FIFO (remainder stays pending), delivered ids appear in codex
  `unlocked`, each page's `note` is non-empty and deterministic
  (same inputs → same text), per-kind pools each produce a note,
  `SetSuspect` flips persistently, store To/FromJson round-trips
  (incl. suspect flags), bad schema / non-array pages / over-cap
  reject, drained-id-not-in-library fails `field`, maxPages=0
  delivers nothing.
- `ctest` green incl. `gameplay_dep_guard`.

## Dev Agent Record

**Implemented 2026-10-05** (MinGW via `C:\MingGoRTS` junction):

- `Campaign/Narrative/Buchao.{h,cpp}` — `BuchaoPage`,
  `BuchaoStore` (To/FromJson, dedupe first-wins, `SetSuspect`),
  `DeliverBuchao`, `ComposeMarginalia`. Six 史官體 clause pools,
  `%N`/`%P` substitution only; pick is FNV-1a(entry.id ‖
  chapterIndex LE) % size — replay-stable.
- Design refinement during implementation: `DeliverBuchao`
  preflights capacity AND library resolution for the whole
  drain before `TakePending` mutates the codex — a `field`
  failure moves nothing (queue + store untouched).
- The 6.6-dependency deviation was resolved as designed:
  suspect flag shipped scoped to `BuchaoPage`; recorded in
  `deferred-work.md`.
- **2026-10-06 revision**: the pending queue gained
  `PendingPage{id,kind,detail}` provenance (10.2 review
  decision landed mid-story). `ComposeMarginalia` now takes
  the `PendingPage` — `%P` substitutes `detail`, the clause
  pool keys on the recorded `kind` wire id (new public
  `UnlockKindName`/`UnlockKindFromName` pair in Bencao.h,
  matching the `ClaimKindName` convention).

**Verification (MinGW, `build-mingw`):**
`potato_test_bencao` 55/55 PASS (14 → 36 → 55 pins); focused
`ctest` 5/5 green incl. `gameplay_dep_guard`,
`potato_test_ledger`, `potato_test_campaign`,
`potato_test_mintclaim`. MSVC not verified.
