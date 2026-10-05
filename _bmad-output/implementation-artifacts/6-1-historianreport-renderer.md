# Story 6.1 — HistorianReport Renderer

> Epic 6 — Narrative Systems (C) · Battle events → 史官體 prose ·
> **Status: done**

## Story (from epics.md)

As a player,
I want post-battle reports in 史官體 with elliptical counts and
omission counters,
so that the account is literature, not a log dump.

## Acceptance Criteria

- **Given** a resolved battle's event list, **when** the report
  renders, **then** it uses clause pools (模糊數詞, not numbers)
  and always prints the omission count **and** rendering is
  deterministic from the same event list.

## Context

`Campaign/Ledger/HistorianReport` is the *integrity* surface —
audit counts, chain status, omission confession for ledger
entries. Story 6.1 is the sibling document: the battle's
`SimEvent` list narrated in the historian register. Narrative
design pins the voice: impersonal, third-person, omits numbers
(「傷亡甚多」 never 「437 casualties」), terse, omissions as style.

## Design

`Campaign/Narrative/BattleReport.{h,cpp}`:

```cpp
std::string RenderBattleReport(
    std::span<const Gameplay::SimEvent> events, int historianSide,
    std::span<const std::uint8_t> seedLevels = {});
```

- **EllipticalCount(n)** — the numeral ban: 0 → omitted (the
  historian says nothing rather than zero), 1–2 → 「一二」, 3–9 →
  「數」, 10–49 → 「數十」, 50–199 → 「百餘」, ≥200 → 「不可勝計」.
- **Clause pools** — each narratable kind has 2 phrasing variants;
  pick = count parity (deterministic, no PRNG in the text layer —
  same rule as MythLog).
- **Perspective** — `historianSide` splits deeds: 王師 vs 敵.
  Side-less events (InfiltrationChanged, MythInvasion) narrate
  impassively — the god's acts belong to no banner.
- **Omissions** — events beneath chronicle notice count toward the
  confession: CardFired (doctrine churn), sustain-beat
  MythInvasions (aux=2 heartbeats after the first), unknown kinds.
  Always prints 本報告省略 N 項 — N=0 is a valid confession.
- **Close** — ResultDeclared renders by CloseReason: Wipe 盡墨 /
  Concede 請降 / Stalemate 相持而解; winner side → 王師奏捷 /
  王師敗績 / 兩軍罷兵.

`seedLevels` (carry-in `MythField::Levels()`) seeds the direction
baseline: `InfiltrationChanged` carries only the NEW level, so
without it a pacification of seeded haunted ground misreads as a
fresh incursion — the chronicle would lie. (Review fix.)

## Implementation Tasks

- [x] `Campaign/Narrative/BattleReport.{h,cpp}` — fold + render
- [x] Tests: elliptical bands, clause-pool determinism, omission
      confession always present, side attribution, close lines,
      infiltration direction w/ carry-in baseline
- [x] Review (three passes) — incursion/pacify direction bug caught
      and fixed via seedLevels; sustain heartbeats → confession;
      myth-verb journal noise silent
- [x] Story → done, sprint-status sync, commit

## Validation

- `potato_test_ledger` — bands, clause parity, digit ban, omission
  count, side attribution, all four close lines, lacuna,
  seed-baseline direction pins
- ctest 19/19 + dep_guard
