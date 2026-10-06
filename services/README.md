# MingGoRTS Backend Services

Spring Cloud microservice scaffold for the MingGoRTS (Potato Engine) game data.
The C++ engine is untouched — these services are standalone consumers/emitters
of the engine's `potato.*` JSON schemas.

## Modules

| Module | Port | Role |
|--------|------|------|
| `discovery-server` | 8761 | Eureka service registry |
| `api-gateway` | 8080 | Spring Cloud Gateway edge (`lb://` routing via Eureka) |
| `roster-service` | 8081 | Character cards (`potato.character_card/1`) + rosters (`potato.roster/1`) |
| `replay-service` | 8082 | Battle replays (`potato.battle_replay/1`) |

Persistence is embedded H2 with JPA, file-backed at `services/data/<svc>.mv.db`
(relative to the directory you launch the jars from — launch from the repo
root). Uploads survive restarts; no external DB server, no Docker, no auth.

## Build

Requires JDK 21+ (built/verified with JDK 23) and Maven 3.9+.

```bat
:: Windows cmd
set JAVA_HOME=C:\Program Files\Java\jdk-23
mvn -f services/pom.xml clean package
```

```bash
# Git Bash
JAVA_HOME="/c/Program Files/Java/jdk-23" mvn -f services/pom.xml clean package
```

Expected: `BUILD SUCCESS` for all four modules, tests green.

## Run order

Start from the **repo root** (so `services/data/` resolves correctly):

```bat
java -jar services\discovery-server\target\discovery-server-1.0.0.jar
java -jar services\roster-service\target\roster-service-1.0.0.jar
java -jar services\replay-service\target\replay-service-1.0.0.jar
java -jar services\api-gateway\target\api-gateway-1.0.0.jar
```

1. `discovery-server` first — the others register with it.
2. `roster-service` / `replay-service` — appear in the Eureka console
   (http://localhost:8761) within ~30 s.
3. `api-gateway` last — it resolves `lb://` routes from the registry.

On first start, `roster-service` seeds its card catalog from bundled verbatim
copies of every `potato.character_card/1` document under
`assets/cards/{roc,ija,avatar}` (`roster-service/src/main/resources/seed/`).

## API

All endpoints are reachable directly (`:8081` / `:8082`) or through the gateway
(`:8080`).

| Endpoint | Behavior |
|----------|----------|
| `GET /api/cards` | 200 + JSON array of `potato.character_card/1` documents |
| `GET /api/cards/{id}` | 200 + card JSON; 404 for unknown id |
| `POST /api/rosters` | Body = `potato.roster/1` doc → 201 + `{"id":N}`; 400 on wrong schema or non-array `entries` |
| `GET /api/rosters` | 200 + metadata list (`id`, `entryCount`, `savedAt`) |
| `GET /api/rosters/{id}` | 200 + full roster JSON; 404 unknown |
| `POST /api/replays` | Body = `potato.battle_replay/1` doc → 201 + `{"id":N}`; 400 on wrong schema / missing `events` array / body > 2 MB |
| `GET /api/replays` | 200 + metadata list (`id`, `eventCount`, `uploadedAt`) — not full event bodies |
| `GET /api/replays/{id}` | 200 + full replay JSON; 404 unknown |

## Example curl

```bash
# List seeded cards through the gateway
curl http://localhost:8080/api/cards

# One card
curl http://localhost:8080/api/cards/roc_chiang_kaishek

# Upload a replay
curl -X POST http://localhost:8080/api/replays \
  -H "Content-Type: application/json" \
  -d '{"schema":"potato.battle_replay/1","events":[{"t":1.0,"event":"test"}]}'

# Rejected (400) — wrong schema tag
curl -X POST http://localhost:8080/api/replays -d '{"schema":"wrong/1"}'

# Replay metadata list / full document
curl http://localhost:8080/api/replays
curl http://localhost:8080/api/replays/1

# Save a roster
curl -X POST http://localhost:8080/api/rosters \
  -H "Content-Type: application/json" \
  -d '{"schema":"potato.roster/1","entries":[{"name":"Squad A","rank":"lt","squad":"1st","team":0,"alive":true,"deathTime":-1.0,"relic":"","art":""}]}'

# Eureka console
curl http://localhost:8761
```
