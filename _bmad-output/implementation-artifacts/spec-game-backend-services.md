---
title: 'Game backend services (Spring Boot + Spring Cloud)'
type: 'feature'
created: '2026-09-17'
status: 'done'
route: 'dispatch'
baseline_commit: '9c36aa823383c71f9a43d6c220ec3f8ef3b4d73a'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** MingGoRTS is a standalone C++ engine — all game data (rosters, battle replays, character cards) lives as local JSON files. There is no server side for sharing/aggregating that data.

**Approach:** Add a Maven multi-module Spring Cloud microservice scaffold under `services/`: Eureka discovery server, Spring Cloud Gateway edge, and two business services (`roster-service`, `replay-service`) exposing REST APIs that consume/emit the engine's existing `potato.*` JSON schemas. Persistence via embedded H2 + Spring Data JPA (file-backed so uploads survive restarts); the C++ engine is not modified.

**Decisions (from human):** Engine HTTP client — NO, services are standalone this round (engine integration deferred). Persistence — H2 embedded DB with JPA, file-backed at `services/data/` (survives restart; not pure in-memory).

## Boundaries & Constraints

**Always:**
- All new code lives under `services/` — a self-contained Maven reactor (`services/pom.xml` as parent). No changes to the C++ engine, `CMakeLists.txt`, `external/`, or `build/`.
- Java 17+ toolchain — use the installed JDK 23 at `C:\Program Files\Java\jdk-23` (`JAVA_HOME` at build time; do not change the machine-global `JAVA_HOME` or `PATH`).
- Spring Boot 3.4.x + Spring Cloud 2024.0.x via Maven BOMs in the parent pom; Java package root `com.potato.rts`.
- Identifiers/class names/comments in English; Traditional Chinese only in user-facing docs/log messages if at all.
- JSON payloads must validate against the existing engine schemas before accepting writes (reject malformed `schema` tags, missing required fields → HTTP 400).

**Never:**
- No external DB server, no Docker, no auth — H2 embedded file DB only (`jdbc:h2:file:./services/data/<svc>`).
- No C++ changes, no HTTP client in the engine, no CI workflow changes.
- Do not vendor Java dependencies — Maven Central only.
- Do not modify `assets/` data files; services may read them at runtime as seed data but never write to them.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| CARD_LIST | GET /api/cards | 200 + array of character cards (schema `potato.character_card/1`) | N/A |
| CARD_GET | GET /api/cards/roc_chiang_kaishek | 200 + card JSON | 404 unknown id |
| REPLAY_UPLOAD | POST /api/replays, body = `{"schema":"potato.battle_replay/1","events":[...]}` | 201 + assigned id | 400 wrong schema tag / missing events array / body >2MB |
| REPLAY_LIST | GET /api/replays | 200 + metadata list (id, eventCount, uploadedAt) — not full event bodies | N/A |
| REPLAY_GET | GET /api/replays/{id} | 200 + full replay JSON | 404 unknown id |
| ROSTER_SAVE | POST /api/rosters, body = `potato.roster/1` JSON | 201 + assigned id | 400 wrong schema / entries not array |
| GATEWAY_ROUTE | GET {gateway}/api/cards | Proxied to roster-service via `lb://` discovery | 503 when service down |
| DISCOVERY | GET localhost:8761 | Eureka console lists registered services | N/A |

</frozen-after-approval>

## Code Map

- `Gameplay/Roster.cpp` lines ~57-100 — `potato.roster/1` writer/reader: `entries[] = {name, rank, squad, team, alive, deathTime, relic, art}`
- `Gameplay/BattleRecorder.cpp` lines ~46-78 — `potato.battle_replay/1` writer/reader: `events[] = {t, event}`
- `assets/cards/roc/roc_chiang_kaishek.json` — canonical `potato.character_card/1` example (id, name, courtesyName, epithet, rarity, faction, role, personality, stats, signatureDoctrine, portrait, lore, historicalBattle, art)
- `assets/maps/duanqiao.json` — `potato.battle_map/1` (out of scope for this spec, schema reference only)
- `AGENTS.md` — repo policy (conventional commits, no `external/`/`build/` edits); Java tree is exempt from the banned-C-function scan but keep it out of `services/**` anyway
- Nothing under `services/` exists yet — all new files. No existing Java code anywhere in the repo.

## Tasks & Acceptance

**Execution:**
- [x] `services/pom.xml` -- parent pom: packaging=pom, modules list, `spring-boot-starter-parent` 3.4.x as parent + `spring-cloud-dependencies` 2024.0.x BOM in dependencyManagement, `maven.compiler.release=21` (JDK 23 compiles it fine) -- RATIONALE: single build entry, version alignment
- [x] `services/discovery-server/` -- module pom + `DiscoveryServerApplication` (@EnableEurekaServer) + `application.yml` (port 8761, `registerWithEureka/fetchRegistry=false`) -- RATIONALE: Spring Cloud service registry
- [x] `services/api-gateway/` -- module pom (spring-cloud-starter-gateway, eureka client) + `ApiGatewayApplication` + `application.yml` (port 8080, routes `/api/cards/**`,`/api/rosters/**` → `lb://roster-service`; `/api/replays/**` → `lb://replay-service`) -- RATIONALE: single entry point + client-side load balancing via discovery
- [x] `services/roster-service/` -- pom (web, data-jpa, h2, eureka client, validation) + `RosterServiceApplication` + `Card`/`Roster` JPA entities matching the schemas + repositories + `RosterController` (endpoints above) + `CommandLineRunner` seeding 2-3 built-in cards + `application.yml` (port 8081, `jdbc:h2:file:./services/data/roster`) -- RATIONALE: owns character-card and roster domains with H2 persistence
- [x] `services/replay-service/` -- pom (web, data-jpa, h2, eureka client, validation) + `ReplayServiceApplication` + `Replay` entity (id, schemaVersion, eventCount, uploadedAt, payload JSON blob) + repository + `ReplayController` (upload validates `schema` field + events array + 2MB cap; list returns metadata only) + `application.yml` (port 8082, `jdbc:h2:file:./services/data/replay`) -- RATIONALE: owns battle-replay domain with H2 persistence
- [x] `services/*/src/test/...` -- `@WebMvcTest` slice tests covering the I/O matrix rows (400 on bad schema, 404 on unknown id, 201 on valid upload) -- RATIONALE: edge cases must be pinned by tests, not manual checks
- [x] `services/README.md` -- build/run instructions: `JAVA_HOME=jdk-23 mvn package`, start order (discovery → services → gateway), example curl commands -- RATIONALE: new toolchain needs a runbook
- [x] `.gitignore` -- add `services/**/target/` and `services/data/` for this tree only -- RATIONALE: Maven output and H2 db files must not enter git

**Acceptance Criteria:**
- Given JDK 23 as JAVA_HOME, when `mvn -f services/pom.xml package` runs, then all modules compile and tests pass.
- Given discovery-server running, when roster-service and replay-service start, then both register in Eureka within 30s.
- Given all four services running, when `GET localhost:8080/api/cards` is called through the gateway, then it returns 200 with a JSON array containing seeded cards.
- Given replay-service running, when a valid `potato.battle_replay/1` body is POSTed, then 201 with an id is returned and `GET /api/replays/{id}` returns the same events.
- Given a replay was uploaded, when replay-service restarts, then `GET /api/replays/{id}` still returns it (H2 file persistence).

## Implementation Notes

- Implementation was found already complete in the working tree (parallel run of this spec); the implementation agent audited every file against the spec instead of rewriting — no edits needed.
- All modules verified live: `mvn clean package` BUILD SUCCESS, 16 @WebMvcTest tests green; Eureka registration confirmed; gateway proxy + 503-on-downstream-down verified via `GatewayErrorHandler` (maps upstream connect/timeout failures to 503); H2 restart persistence confirmed for replays.
- Seed cards are byte-identical copies of `assets/cards/` documents under `roster-service/src/main/resources/seed/`, loaded by `CardSeeder` (idempotent — skips existing ids).
- Note: `services/data/*.mv.db` holds verification test rows; gitignored, delete for a clean slate.

## Spec Change Log

## Review Triage Log

- README JDK 17+ vs pom release=21 — medium (docs lie about toolchain). → patch
- Seed bundles only 3 of ~31 card documents; README overstates catalog — low → patch (bundle all card JSONs)
- CardSeeder: no update path, stream leak, malformed-seed crash, empty-id accepted, save race — medium cluster → patch
- CWD-relative `jdbc:h2:file:./services/data/...` forks DB per launch dir — medium → patch (`${DB_DIR:...}` placeholder)
- No body-size cap on rosters; replay cap checked after full buffering — medium → patch (cap + Content-Length pre-check)
- `findAll()` loads CLOB payloads for metadata lists — medium → patch (JPQL projection)
- Hand-concatenated JSON error/list bodies — escapable/corruptible — medium → patch (ObjectMapper serialization)
- Element-level field validation missing (spec required `missing required fields → 400`) — high (accepts `[{}]`) → patch
- Gateway: no httpclient timeouts; TimeoutException branch dead; narrow exception set; HttpStatus.valueOf throws on non-standard codes — medium → patch
- No tests: roster read endpoints, GatewayErrorHandler, CardSeeder — high (contracted 503 + only card write-path unverified) → patch
- Hardcoded localhost:8761 everywhere — low → patch (`${EUREKA_DEFAULT_ZONE:...}`)
- UTF-16/32 upload parses via Jackson then stored as UTF-8 mojibake — low → patch (re-serialize canonical UTF-8)
- Duplicate JSON keys: validate-last vs store-both — low → patch (STRICT_DUPLICATE_DETECTION)
- `toMeta` emits `"savedAt":"null"` for null timestamps — low → patch (fold into JsonNode serialization)
- Entity ctor visibility inconsistent (Card protected, others public) — cosmetic → patch (align protected)
- DELETE endpoints absent (retention story) — out of frozen intent, adds public surface → defer
- Shared `services-common` module for duplicated helpers — refactor beyond this change → defer
- ddl-auto=update unmanaged schema drift / no Flyway — acceptable for scaffold → defer
- No Maven invocation in CI — spec explicitly excluded CI changes → defer (documented)

## Design Notes

Service split rationale: roster-service owns *static-ish* game data (character cards = collectible definitions, rosters = per-battle personnel records). replay-service owns *append-heavy* battle output. This mirrors how the engine writes them (Roster/BattleRecorder are separate classes). A future `matchmaking-service` or `leaderboard-service` can join the same Eureka registry without touching these.

Schema validation rule: accept only `schema` values the engine emits (`potato.character_card/1`, `potato.roster/1`, `potato.battle_replay/1`); reject unknown/major-version-mismatch tags with 400 — keeps the Java side honest about being a consumer of engine formats, not a new format authority.

## Verification

**Commands:**
- `set JAVA_HOME=C:\Program Files\Java\jdk-23 && mvn -f services/pom.xml clean package` -- expected: BUILD SUCCESS, all modules, tests green
- `java -jar services/discovery-server/target/*.jar` then `java -jar services/roster-service/target/*.jar` then `java -jar services/replay-service/target/*.jar` then `java -jar services/api-gateway/target/*.jar` -- expected: all four up; `curl localhost:8761` shows registry; `curl localhost:8080/api/cards` returns 200 JSON array
- `curl -X POST localhost:8080/api/replays -H "Content-Type: application/json" -d '{"schema":"potato.battle_replay/1","events":[{"t":1.0,"event":"test"}]}'` -- expected: 201 + id
- `curl -X POST localhost:8080/api/replays -d '{"schema":"wrong/1"}'` -- expected: 400
