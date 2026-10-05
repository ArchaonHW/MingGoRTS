---
baseline_commit: NO_VCS
---

# Story 11.1 — MintClaim Schema & Outbox

> Epic 11 — 鑄鏈存證 Mint & Anchor (MT) · `potato.mintclaim/1` ·
> **Status: done**

## Story (from epics.md)

As a developer,
I want `potato.mintclaim/1` claim documents — content-bound to the
ledger seal and the battle record root — written atomically into an
outbox directory,
So that an external process can pick up mintable events without the
engine ever touching a network.

## Acceptance Criteria

- **Given** a settled chapter's anchors (record_root, ledger
  count+tip), **when** a claim is emitted, **then** the
  `potato.mintclaim/1` doc binds {kind, chapter, record_root,
  ledger_count, ledger_tip, payload} and commits to
  `<outbox>/<id>.json` via tmp→rename **and** a malformed or
  bad-schema claim file rejects on read without disturbing the
  outbox **and** claim ids are deterministic — re-emitting the same
  settlement produces the same file (idempotent, matching
  ResolveAftermath's record_root idempotency).

## Context

Design authority:
`_bmad-output/forge/metaverse-currency/forged-idea.md` (locked
decisions) and
`_bmad-output/planning-artifacts/sprint-change-proposal-2026-10-05.md`
(approved). Architecture: D-ARCH-9 — the chain boundary is a
versioned-JSON outbox; `Campaign/Chain/` writes claims, the external
`Tools/MintRelayer/` (Story 11.4) does all web3 work. The engine has
**no networking** — this story is pure file I/O at the campaign
boundary.

This story lands the *container only*: claim schema + outbox
write/read. **Emission wiring** (calling `Emit` from the aftermath
ceremony behind a CrossCheck pass) is Story 11.3 — do not touch
`Aftermath.cpp` here. **Merkle root** is 11.2 — the claim only needs
to *carry* `ledger_count`/`ledger_tip`, not compute a Merkle tree.
**The relayer** is an external process (11.4) — nothing here shells
out.

### Precedents to mirror

- `SaveSystem` (Campaign/Save/SaveSystem.h): tmp→rename atomic
  commit, `.tmp`/`.json` extensions, single-writer-per-dir honesty
  note, MOVEFILE_REPLACE_EXISTING on Windows (MSVC + MinGW GCC≥11
  verified). MintOutbox is per-file (one file per claim id), not
  slot-based — implement the same write pattern with claim-id file
  names.
- `CrossCheck` (Campaign/Ledger/CrossCheck.h): `record_root` is a
  16-lowercase-hex string (`RecordRootTag`/`ParseRecordRootTag`
  conventions) — reuse that spelling on the wire.
- `Ledger` (Campaign/Ledger/Ledger.h): `Tip()`, `Seal()`,
  `SealHash(count, tip)` exist already — the claim stores `count`
  and `tip` verbatim so the relayer can recompute the seal itself.
  uint64 hash values are bit-cast to int64 on the wire (Ledger.h
  wire note) — follow the same convention; a literal ≥2^63 parses
  as Real and must never appear.

## Design

`Campaign/Chain/MintClaim.{h,cpp}` — claim model + schema guard.
`Campaign/Chain/MintOutbox.{h,cpp}` — directory writer/scanner.
Chain is a new Campaign submodule (D-ARCH-9); it is campaign-layer
file I/O, never `Gameplay/` (nothing here may be includable from the
sim).

### Wire shape — `potato.mintclaim/1`, one file per claim

```json
{
  "schema": "potato.mintclaim/1",
  "id": "settlement-0123abcdef456789",
  "kind": "chapter_settlement",
  "chapter": 7,
  "record_root": "0123abcdef456789",
  "resolution": "governance",
  "ledger": {"count": 412, "tip": -2606419513542532627}
}
```

Achievement claim:

```json
{
  "schema": "potato.mintclaim/1",
  "id": "achievement-zero_combat-0007",
  "kind": "achievement",
  "chapter": 7,
  "achievement": "zero_combat",
  "ledger": {"count": 412, "tip": -2606419513542532627}
}
```

### Model

```cpp
enum class ClaimKind : std::uint8_t {
    ChapterSettlement, // "chapter_settlement"
    Achievement,       // "achievement"
};

struct MintClaim {
    ClaimKind kind = ClaimKind::ChapterSettlement;
    std::int64_t chapter = 0;
    std::uint64_t recordRoot = 0;      // settlement only
    std::string resolution;            // settlement only: military/governance/subversion/defeat
    std::string achievement;           // achievement only: trigger key
    std::uint64_t ledgerCount = 0;     // Ledger::Size()
    std::uint64_t ledgerTip = 0;       // Ledger::Tip()
};

// Deterministic id — pure function of content:
//   settlement → "settlement-" + record_root hex
//   achievement → "achievement-" + key + "-" + zero-padded chapter
// NOTE: '-' separators — the id IS the outbox filename and ':' is
// illegal in Windows filenames; achievement keys are [a-z0-9_] only.
std::string ClaimId(const MintClaim& c);
```

`MintClaim::ToJson()` emits the doc (including recomputed `id`);
`MintClaim::FromJson(doc)` validates AND requires the stored `id`
to equal `ClaimId(content)` — a mismatched id is a tampered claim.

```cpp
class MintOutbox {
public:
    static constexpr std::string_view CLAIM_EXT = ".json";
    static constexpr std::string_view TMP_EXT   = ".tmp";
    static constexpr std::size_t MAX_CLAIMS = 4096;

    explicit MintOutbox(std::filesystem::path dir);

    // Writes `<dir>/<id>.tmp` then renames over `<dir>/<id>.json`.
    // Idempotent: same claim → same id → same file (byte-identical
    // content; a re-emit overwrites harmlessly). Creates `dir` if
    // absent. Never partially commits.
    Gameplay::Result<std::filesystem::path> Emit(
        const MintClaim& claim) const;

    // Reads every `<dir>/*.json`, schema-gates each through
    // FromJson; a bad file lands in `rejected[]` (name + reason)
    // and the scan continues. Unreadable dir → `ok=false, "io"`.
    struct ScanResult {
        std::vector<MintClaim> claims; // sorted by id
        std::vector<std::pair<std::string, std::string>> rejected;
    };
    Gameplay::Result<ScanResult> Scan() const;
};
```

### Validation rules (file-is-untrusted)

- `schema` must equal `potato.mintclaim/1` exactly → reject `schema`.
- `id` required and must equal `ClaimId(content)` → reject `id`
  (the binding IS the tamper check).
- `kind` must be a known string → reject `kind`.
- `chapter` ≥ 0 and ≤ `MAX_CHAPTERS` (CampaignState bound) → `field`.
- Settlement claims: `record_root` required, 16 lowercase hex →
  `field`; `resolution` one of military/governance/subversion/
  defeat → `field`; `achievement` must be absent or empty.
- Achievement claims: `achievement` required, ≤64 chars, ASCII
  `[a-z0-9_:]`-ish → `field`; `record_root`/`resolution` absent.
- `ledger.count` ≥ 0, `ledger.tip` int64 (bitcast uint64) → `field`.
- `MAX_CLAIMS` bound on Scan; `MAX_*` on all string fields (mirror
  Ledger/ChapterLibrary constants style).
- Unknown extra members tolerated (forward-compat convention); no
  closed-schema guard needed here.

## Implementation Tasks

- [x] `Campaign/Chain/MintClaim.{h,cpp}` — `ClaimKind`,
      `MintClaim`, `ClaimId`, `ToJson`/`FromJson` + validation
- [x] `Campaign/Chain/MintOutbox.{h,cpp}` — `Emit` (tmp→rename),
      `Scan` (per-file rejection, sorted claims)
- [x] `Examples/potato_test_mintclaim.cpp` — temp-dir fixtures
      (`fs::temp_directory_path()` + `std::ofstream` + `remove_all`
      cleanup, the `potato_test_bencao` pattern)
- [x] CMake: `add_executable(potato_test_mintclaim …)` +
      `add_test` — copy the `potato_test_bencao` block shape
      (links `PotatoCampaign`; glob `CONFIGURE_DEPENDS` picks up the
      new sources automatically)
- [x] Review (three passes), sprint-status sync

## Dev Agent Record

### Completion Notes List

- One design deviation landed during implementation: claim ids
  use `-` separators (`settlement-<hex>`, `achievement-<key>-<ch>`),
  NOT the `:` separators sketched in the proposal — the id IS the
  outbox filename and `:` is illegal in Windows filenames.
  Achievement keys are likewise bounded to `[a-z0-9_]` (no `:`,
  no `-`). epics.md/story file updated to match.
- `resolution` validates against the canonical `ResolutionName`
  spellings (battle_victory/governance_victory/defeat) plus
  `subversion` for Epic 7's negotiated outcomes — a bounded set,
  not free text.
- `record_root` travels as a 16-lower-hex string (CrossCheck
  spelling) — dodges the ≥2^63 Real-parse trap that bites
  `ledger.tip`/`ledger.count`, which ride int64 bit-cast per the
  Ledger.h wire note (verified: `0xdbdc…eed` → `-2604144523890565395`).
- `MintOutbox::Emit` is byte-identical idempotent (same claim →
  same file, harmless overwrite) — no dedup bookkeeping, per the
  record_root idempotency precedent in ResolveAftermath.
- Build verified: MinGW (Ninja, `C:\MingGoRTS` junction) — target
  + ctest `potato_test_mintclaim` + `gameplay_dep_guard` green.
  MSVC not verified this session.

### File List

- `Campaign/Chain/MintClaim.h` (new)
- `Campaign/Chain/MintClaim.cpp` (new)
- `Campaign/Chain/MintOutbox.h` (new)
- `Campaign/Chain/MintOutbox.cpp` (new)
- `Examples/potato_test_mintclaim.cpp` (new)
- `CMakeLists.txt` (test target registration)

## Dev Notes — guardrails

- **Layering**: `Campaign/` may include only `Gameplay/` public
  headers (`Json/JsonValue.h`, `Result.h`); never Rendering/GUI —
  `scripts/CheckGameplayDeps.ps1` enforces. `PotatoCampaign` already
  links `PotatoGameplay`; **do not link `PotatoEngine`** (MinGW
  breakage — AGENTS.md pitfall).
- **No networking, no subprocess** — this module writes files only.
  Anything that opens a socket or spawns a process is out of scope
  by architecture (D-ARCH-9).
- **Errors**: `Result<T>{value,error,reason}` at all boundaries;
  never throw, never mutate `out` on rejection.
- **No third-party JSON** — `JsonValue` only (`FindString`/`AsInt`/
  `AsString`/`Items`/`Members`); check `Has`/`GetType` for required
  fields.
- **Hash on the wire**: uint64 hash-family values bit-cast to int64
  for JSON emit (Ledger.h wire note) — `ledger.tip` and
  `record_root` (as hex string, NOT a number — the hex spelling is
  the CrossCheck convention and dodges the ≥2^63 Real-parse trap
  entirely).
- **Determinism**: no PRNG, no wall clock — claim ids derive from
  content; ordering is chapter/seq, not time.
- **Idempotency**: `Emit` of an identical claim is a byte-identical
  overwrite — safe by construction; do NOT add dedup bookkeeping.
- **Naming**: PascalCase files/types/methods, camelCase members,
  `UPPER_SNAKE` constants, `Potato::Campaign` namespace.
- **Build/verify**: MSVC path per AGENTS.md (`cmake -B build` …);
  headless via `-DPOTATO_BUILD_GUI=OFF`; repo path has CJK — build
  through the `C:\MingGoRTS` junction for MinGW; `uv`/`python`/`git`
  are NOT on PATH here — don't script around them.

## Validation

- `potato_test_mintclaim` pins: settlement claim emit → file exists
  with expected id name; byte-identical re-emit; achievement claim
  round-trip; stored-id ≠ recomputed-id rejection; bad schema;
  unknown kind; missing record_root / bad hex; missing achievement;
  bad resolution string; kind-crossed field rejection
  (achievement claim carrying record_root); `ledger.tip` bitcast
  round-trip; Scan with one poisoned file → claims ok + rejected
  listed; unreadable dir → `io`.
- `ctest` green incl. `gameplay_dep_guard`.
