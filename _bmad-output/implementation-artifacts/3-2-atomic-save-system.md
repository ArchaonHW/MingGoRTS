# Story 3.2: Atomic Save System

## Status: done

## Story

As a player,
I want saves written atomically and bad files rejected without
touching my game,
So that a crash never eats my campaign.

## Acceptance Criteria

1. **Given** a save slot write,
   **When** interrupted mid-write,
   **Then** the previous save remains valid (tmp→rename)
   **And Given** a file with bad schema/version,
   **When** loaded,
   **Then** it is rejected with reason and current state is
   unmodified.

## Design

`Campaign/Save/SaveSystem` — the durability layer over
`CampaignState` (3.1 stays file-agnostic; this story owns the
disk boundary):

- `Save(slot, state)`: `state.ToJson().Emit()` → write to
  `<dir>/save_<slot>.tmp` → flush → explicit `close()` (close-time
  write errors observable) → `std::filesystem::rename` to
  `save_<slot>.json`. A crash mid-write leaves only a truncated
  `.tmp`; the previous `.json` is untouched. On Windows,
  `std::filesystem::rename` maps to `MoveFileExW` with
  `MOVEFILE_REPLACE_EXISTING` on MSVC and MinGW GCC ≥ 11 (verified
  toolchain: GCC 16.1) — overwrite of a prior save is atomic.
  **A rename failure never touches the committed save** — the
  complete new data stays in `.tmp` for the next attempt.
- `Load(slot)`: delegates to `Gameplay::Json::Load` (64 MiB cap,
  read-error check, UTF-16 reject, BOM strip, schema gate) →
  `CampaignState::FromJson`. Every failure is `Fail(errorClass,
  reason)`; the caller's `CampaignState` is structurally
  untouchable (load returns a fresh value).
- Slot names are canonical: `save_0.json` … `save_7.json`
  (`MAX_SLOTS=8`, out-of-range rejected before any I/O).
  `ListSlots()` reports present regular files only.

## Tasks / Subtasks

- [x] (AC: 1) Task 1 — `SaveSystem.{h,cpp}`
- [x] Task 2 — tests: save→load, overwrite, crash-simulated
    stale .tmp keeps prior save valid, corrupt/torn file
    rejected (schema + parse classes), slot bounds, missing file
- [x] Task 3 — story/sprint docs; commit
- [x] Task 4 — three-layer review

## Dev Notes

- File I/O lives only here — sim/tick paths remain I/O-free.
- Boundary honesty: this is **process-crash atomicity, not
  power-loss durability** (no fsync on file or directory).
- Single writer per save dir assumed: two concurrent Saves to one
  slot share the tmp name and can commit torn content.
- Review fix (all three layers flagged): an initial
  remove+rename fallback could delete the last good save on any
  transient rename failure — removed; rename failure now returns
  Fail with the old save intact and the new data recoverable in
  `.tmp`.
- Residual: `Load` makes no `.tmp` recovery attempt — a torn
  `.tmp` is ignored, a complete one is recoverable manually.

## File List

- `Campaign/Save/SaveSystem.h` (new)
- `Campaign/Save/SaveSystem.cpp` (new)
- `Examples/potato_test_campaign.cpp` (+3.2 test block)
- `_bmad-output/implementation-artifacts/3-2-atomic-save-system.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
