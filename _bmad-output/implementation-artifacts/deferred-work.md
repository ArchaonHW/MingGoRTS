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
