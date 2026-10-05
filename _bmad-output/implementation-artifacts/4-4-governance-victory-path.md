# Story 4.4: Governance Victory Path

## Status: done

## Story

**As a** player,
**I want** to win chapters by holding 民心≥70 and 秩序≥60 at chapter
end,
**So that** ruling well beats fighting well.

## Acceptance Criteria

1. Given a chapter reaching its end condition,
   When accumulators meet thresholds,
   Then the chapter resolves as governance victory
   And the report renders it in chronicle voice, not as a battle
   rout.

## Dev Notes

- GDD line ~86: three chapter-victory paths — military, governance
  (民心 ≥ 70 and 秩序 ≥ 60 held at chapter end), subversion. This
  story implements the governance path plus the resolution seam the
  others plug into.
- **Governance thresholds override the field result**: a chapter
  whose ledger holds 民心≥70 ∧ 秩序≥60 closes as governance victory
  even when the last battle was lost — the boldest reading of
  「以筆代兵，以治為勝」. Win-without-fighting is a first-class path
  (GDD pillar).
- **`ConcludeChapter(state, lib, battleWon)`** is the chapter-end
  ceremony: fold `FoldGovernance(state.GetLedger())` → decide
  `ChapterResolution` → **seal the verdict into the ledger** with a
  `resolution:<kind>` + `chapter:<idx>` tagged posting →
  `ResolveAndAdvance`. The verdict is ledger truth: the historian
  folds it, forgers can contest it, audits count it.
- **Fail-closed atomicity**: preflight mirrors `ResolveAndAdvance`'s
  gates plus ledger capacity, so once the seal posts the advance
  cannot fail. Verdict lands in the book BEFORE the chapter counts
  as closed.
- **Seal legs are real bookkeeping** (double-entry demands both
  legs > 0): a token `kSealAmount = 1` —
  - governance victory: `credit 天命 / debit 民心` — 民心一分凝為天命
  - battle victory: `credit 軍威 / debit 物資` — 軍威以物資記
  - defeat: `credit 物資 / debit 軍威` — 收殮得物資，軍威折損
- **`HistorianReport` folds `resolution:` entries** into
  `resolutions[]` (forged/suspect included — suspicion is data) and
  renders each in 史官體:
  `resolution chapter N: governance victory — 以治為勝`,
  `battle victory — 以兵取之`, `defeat — 敗而未絕`.
  Chronicle voice = impersonal historian register per
  narrative-design (epithet lines, not casualty tallies).
- `Defeat` resolution is recorded here; the recovery/continuation
  machinery it routes into is Story 4.5's.
- Per-chapter resolution is durable via the ledger seal — re-folding
  reconstructs it (unlike accumulators at chapter boundaries).
- New module: `Campaign/Governance/Victory.{h,cpp}` beside
  Accumulators.

## Review findings (three-layer: acceptance / edge / independent)

AC audit: PASS. One high, three mediums — all resolved:

- **HIGH (pre-existing 2.4 bug)**: confession marker rendered
  `本埁告省略` — `\xA0\x81` (埁 U+5801) instead of `\xA0\xB1`
  (報 U+5831). Fixed in render AND in the two test assertions that
  had pinned the wrong bytes.
- **MED ×3 (all layers converged)**: `CanClose` was a hand-mirrored
  copy of `ResolveAndAdvance`'s gates — the atomicity promise could
  silently drift. Extracted shared predicate
  `CanResolveChapter(state, lib, error&)` into Progression; both
  paths call it. A post-seal advance failure is now unreachable by
  construction, not by convention.
- **MED**: forged/suspect verdicts rendered byte-identical to
  honest seals. `ResolutionNote` now carries `forged`/`suspect`;
  render annotates ` (forged)` / ` (suspect)` per line — doubt is
  attributable, not just an aggregate counter.
- **MED**: governance seal debits 民心 by the token grain —
  cumulative across chapters (k victories ⇒ effective threshold
  70+k). Documented as deliberate in Victory.h: the seal consumes
  the resource it measures.
- **LOW**: error-class parity (`"campaign"` for complete);
  unknown `resolution:` payloads render sanitized (control bytes →
  `?`, no report-line injection); negative-chapter render guard;
  `SealPosting` intentionally has no `default` (`-Wswitch` is the
  tripwire for new resolutions); doc nits.

Verification: `potato_test_campaign`/`potato_test_ledger` green
(new pins: upward override, seal legs per kind, malformed chapter
payloads, forged/suspect line annotation, sanitized unknown kind,
all `ResolutionName` spellings, `campaign` error class); ctest
18/18; dep guard pass.

## File List

- `Campaign/Governance/Victory.h` (new)
- `Campaign/Governance/Victory.cpp` (new)
- `Campaign/Ledger/Ledger.h` (`TAG_RESOLUTION`/`TAG_CHAPTER`)
- `Campaign/Ledger/HistorianReport.h/.cpp` (resolution fold + render)
- `Campaign/Chapters/Progression.h/.cpp` (`CanResolveChapter` shared
  predicate)
- `Examples/potato_test_campaign.cpp`, `potato_test_ledger.cpp`

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `ChapterResolution`, thresholds,
    `ConcludeChapter` (fold → seal → advance)
- [x] Task 2 — `HistorianReport` resolution fold + chronicle-voice
    render
- [x] Task 3 — tests (threshold boundaries, override on lost field,
    seal contents/order, fail-closed, render lines, forged
    resolution)
- [x] Task 4 — story/sprint docs; commit
- [x] Task 5 — three-layer review
