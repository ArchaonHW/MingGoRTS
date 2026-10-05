---
baseline_commit: NO_VCS
---

# Story 10.1 — Bencao Entry Schema & Registry

> Epic 10 — Bencao Codex (BC) · `potato.bencao/1` ·
> **Status: done**

## Story (from epics.md)

As a developer,
I want `potato.bencao/1` versioned entry files loaded into a boot-time
registry with the six-field anatomy and 8-category taxonomy,
so that herb content is data, not code.

## Acceptance Criteria

- **Given** `potato.bencao/1` files in `assets/bencao/`, **when** the
  campaign boots, **then** entries register read-only with required
  fields validated (正名, 類, 性味歸經, 主治 present; 釋名/集解
  optional; 批註 slots are runtime-bound, never authored) **and** each
  entry carries its own `unlock` trigger manifest in-file (terrain
  kind, ledger-event tag, governance event, myth state, 墮落
  threshold, or chapter-close) plus a `source` citation field (卷/條)
  for the verification record **and** a bad schema/version rejects
  that file without failing the library **and** entry fields carry no
  executable semantics — codex content never reaches the sim.

## Context

Design authority: `_bmad-output/bencao-worldview.md` (§3 entry
anatomy, §4 trigger families, §8 boundaries). The codex is the desk's
second book — a collection-layer narrative system with zero sim
involvement. This story lands the *container only*: schema + registry.
Trigger **evaluation** is 10.2, 補鈔 delivery/批註 binding is 10.3,
rendering is 10.4, content authoring is 10.6 — do not build any of
those here.

`BencaoLibrary` mirrors `ChapterLibrary` (3.3): a boot-time,
immutable, per-file-isolated registry. `ChapterLibrary::Load` is the
direct precedent — sorted-filename iteration, `LoadResult{ok, error,
reason, rejected[]}`, empty dir → empty ok, unreadable dir → `io`.

## Design

`Campaign/Narrative/Bencao.{h,cpp}` — Narrative is where content
systems live (Dossier 6.3, IntelLedger 6.4); the codex is narrative
content, never `Gameplay/` (it must stay out of the sim layer by
construction, not just by discipline).

### Wire shape — `potato.bencao/1`, one file per entry

```json
{
  "schema": "potato.bencao/1",
  "id": "sanqi",
  "category": "shancao",
  "name": "三七",
  "aliases": ["山漆", "金不換"],
  "origin": "生廣西、雲南山峒深處…",
  "nature": "甘、微苦，溫。歸肝、胃經。",
  "indications": "止血散血，定痛…",
  "unlock": {"kind": "ledger_tag", "tag": "first_casualty"},
  "source": "本草綱目·卷十二",
  "lang": {"zh-tw": true}
}
```

### Model

```cpp
enum class BencaoCategory : std::uint8_t {
    Shancao,  // 山草類   — "shancao"
    Xicao,    // 隰草類   — "xicao"
    Ducao,    // 毒草類   — "ducao"
    Manshui,  // 蔓草/水草 — "manshui"
    Gucai,    // 穀菜類   — "gucai"
    Jinshi,   // 金石類   — "jinshi"
    Chongshou,// 蟲獸類   — "chongshou"
    Renbu,    // 人部拾遺 — "renbu"
};

enum class UnlockKind : std::uint8_t {
    Terrain,      // {"kind":"terrain","terrain":"<flag id>"}
    LedgerTag,    // {"kind":"ledger_tag","tag":"<tag>"}
    Governance,   // {"kind":"governance","event":"<id>"}
    MythState,    // {"kind":"myth_state","state":"<id>"}
    Corruption,   // {"kind":"corruption","at_least":<int>}
    ChapterClose, // {"kind":"chapter_close","chapter":<int|-1 any>}
};

struct BencaoEntry {
    std::string id;
    BencaoCategory category;
    std::string name;                    // 正名 — required
    std::vector<std::string> aliases;    // 釋名 — optional
    std::string origin;                  // 集解 — optional
    std::string nature;                  // 性味歸經 — required
    std::string indications;             // 主治 — required
    UnlockKind unlockKind;
    std::string unlockParam;             // tag/terrain/event/state id
    std::int64_t unlockInt = 0;          // corruption.at_least / chapter
    std::string source;                  // 卷/條 citation — required
    // lang: zh-tw required (the entry fields above ARE zh-tw);
    // en deferred per OQ-B3 — an "en" member block is tolerated
    // but ignored for now.
};

class BencaoLibrary {
public:
    static constexpr std::string_view SCHEMA = "potato.bencao/1";
    static constexpr std::size_t MAX_ENTRIES = 1024;
    static constexpr std::size_t MAX_ID_LEN = 64;
    static constexpr std::size_t MAX_NAME_LEN = 64;
    static constexpr std::size_t MAX_TEXT_LEN = 2048; // origin/nature/indications
    static constexpr std::size_t MAX_ALIASES = 16;
    static constexpr std::size_t MAX_SOURCE_LEN = 128;

    static BencaoLoadResult Load(const std::filesystem::path& dir,
                                 BencaoLibrary& out);
    const BencaoEntry* Find(std::string_view id) const;
    std::size_t Size() const;
    const std::vector<BencaoEntry>& Entries() const; // category-then-id order
};
```

### Validation rules (file-is-untrusted, per project policy)

- `schema` must equal `potato.bencao/1` exactly → reject `schema`.
- Required: `id`, `category` (known enum string), `name`, `nature`,
  `indications`, `source`, `unlock` (known kind + required params for
  that kind). Missing/empty/over-bound → reject `field`.
- `批註` is **runtime-bound**: any of the keys `annotation`,
  `marginalia`, `annotations` present at top level → reject `field`
  (closed-schema guard, explicit this once — other unknown fields are
  tolerated per repo convention).
- Duplicate `id` across files → second file rejected.
- Unknown `category`/`unlock.kind` strings → reject (strictness
  precedent: `ReadClause` rejects unknown kinds, not ignores them).
- Per-file isolation: bad file → `rejected[]` entry, library
  continues; unreadable dir → `ok=false, error="io"`.
- Entries stored sorted by (category ordinal, id) — canonical order
  the 10.4 renderer can rely on.

### Explicit non-goals for this story

- **No unlock evaluation** — manifest parses and validates; nothing
  reads campaign/ledger state (10.2's job).
- **No 批註 storage** — no annotation fields or runtime slots (10.3).
- **No rendering** — no RenderText (10.4).
- **No content files** — do not author `assets/bencao/*.json` herb
  entries (10.6); test fixtures live in temp dirs inside the test.
- **Not in `Gameplay/`** — codex is campaign-layer narrative content;
  it must never be includable from the sim.

## Implementation Tasks

- [x] `Campaign/Narrative/Bencao.{h,cpp}` — enums, `BencaoEntry`,
      `BencaoLibrary::Load` + per-file `FromJson` validate
- [x] `Examples/potato_test_bencao.cpp` — temp-dir fixtures
      (`fs::temp_directory_path()` + `std::ofstream` + `remove_all`
      cleanup, the `potato_test_campaign` pattern)
- [x] CMake: `add_executable(potato_test_bencao …)` +
      `add_test` — copy the `potato_test_campaign` block shape
      (links `PotatoCampaign`; glob `CONFIGURE_DEPENDS` picks up the
      new sources automatically)
- [x] Review (three passes), sprint-status sync

## Dev Agent Record

### Completion Notes List

- `BencaoLibrary` mirrors `ChapterLibrary` end to end: sorted-
  filename load, per-file `rejected[]`, `io` on unreadable dir,
  empty dir → empty library.
- One deviation landed during implementation: `MAX_ENTRIES`
  overflow can't be a per-file rejection (ids already committed),
  so overflow fails wholesale (`error="overflow"`) — the file-is-
  untrusted bound still holds.
- `lang` block is optional; when present `zh-tw` must not be
  explicitly `false`. `en` tolerated and ignored (OQ-B3).
- The 批註 guard rejects `annotation`/`annotations`/`marginalia`
  top-level keys; other unknown members ride through per the
  repo's forward-compat convention.
- Build note: the default generator here is Ninja+MinGW via the
  `C:\MingGoRTS` ASCII junction; a full `--build` (ALL target)
  fails on pre-existing `AI/NaturalLanguageProcessing.cpp`
  breakage unrelated to this story — game-layer targets build
  clean. MSVC not verified this session.

### File List

- `Campaign/Narrative/Bencao.h` (new)
- `Campaign/Narrative/Bencao.cpp` (new)
- `Examples/potato_test_bencao.cpp` (new)
- `CMakeLists.txt` (test target registration)

## Dev Notes — guardrails

- **Layering**: `Campaign/` may include only `Gameplay/` public
  headers (`Json/JsonValue.h`, `Result.h`); never Rendering/GUI —
  `scripts/CheckGameplayDeps.ps1` enforces. `PotatoCampaign` already
  links `PotatoGameplay`; **do not link `PotatoEngine`** (MinGW
  breakage — AGENTS.md pitfall).
- **Errors**: `Result<T>{value,error,reason}` at all load boundaries;
  never throw, never mutate `out` on rejection — per-file rejection
  leaves the library untouched (ChapterLibrary contract).
- **No third-party JSON** — parse via `JsonValue` only
  (`FindString`/`AsInt`/`AsString`/`Items`/`Members`; defensive
  accessors return fallbacks, so check `Has`/`GetType` for required
  fields).
- **UTF-8**: `JsonValue` does not validate raw ≥0x80 bytes in strings
  (accepted leniency, deferred from 1.2) — CJK entry text inherits
  it; do not add a validation pass here.
- **Bounds**: every string/array field needs a `MAX_*` wire bound —
  mirror `ChapterLibrary`'s constants style; the file-is-untrusted
  rule means a load must not admit content no write path could
  produce.
- **Naming**: PascalCase files/types/methods, camelCase members,
  `UPPER_SNAKE` constants, `Potato::Campaign` namespace.
- **Determinism**: trivially satisfied — no PRNG, no clock; sorted
  load order is the canonical order.
- **Assets dir**: `assets/` does not exist in-repo yet — the story
  creates the loader, not the directory; tests must not read
  `assets/`.
- **Build/verify**: MSVC path per AGENTS.md (`cmake -B build` …);
  headless via `-DPOTATO_BUILD_GUI=OFF`; repo path has CJK — build
  through the `C:\MingGoRTS` junction for MinGW; exec shell here is
  non-functional (`ls`/`uv` unavailable) — use the file tools, not
  shell scripting, if you need to inspect the tree.

## Validation

- `potato_test_bencao` pins: valid multi-file load (category order),
  each required-field rejection, bad schema, duplicate id, unknown
  category, unknown unlock kind, missing unlock params, 批註-key
  rejection, bounds overflow, empty dir → empty ok, unreadable dir →
  `io` error.
- `ctest` green incl. `gameplay_dep_guard`.
