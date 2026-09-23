package com.potato.rts.roster.web;

import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.potato.rts.roster.model.Card;
import com.potato.rts.roster.model.Roster;
import com.potato.rts.roster.repo.CardRepository;
import com.potato.rts.roster.repo.RosterRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * REST API for character cards and battle rosters.
 *
 * <p>Write endpoints validate the {@code schema} tag against what the engine
 * actually emits; unknown or major-version-mismatched tags are rejected with
 * HTTP 400 so this service never becomes a competing format authority.
 * All response bodies are serialized through Jackson so stored documents or
 * identifiers containing special characters cannot corrupt the output.
 */
@RestController
@RequestMapping("/api")
public class RosterController {

    private static final Logger log = LoggerFactory.getLogger(RosterController.class);

    /** Only schema tag accepted for roster writes. */
    static final String ROSTER_SCHEMA = "potato.roster/1";

    /** Hard cap on write bodies: 2 MiB. */
    public static final int MAX_BODY_BYTES = 2 * 1024 * 1024;

    private final CardRepository cardRepository;
    private final RosterRepository rosterRepository;
    private final ObjectMapper objectMapper;

    public RosterController(CardRepository cardRepository,
                            RosterRepository rosterRepository,
                            ObjectMapper objectMapper) {
        this.cardRepository = cardRepository;
        this.rosterRepository = rosterRepository;
        this.objectMapper = objectMapper;
    }

    /** CARD_LIST: all character cards as a JSON array of card documents. */
    @GetMapping("/cards")
    public List<JsonNode> listCards() {
        List<JsonNode> docs = new ArrayList<>();
        for (Card card : cardRepository.findAll()) {
            JsonNode doc = parseQuietly(card.getPayload());
            if (doc == null) {
                // Skip a corrupt row rather than corrupting the whole list.
                log.warn("Skipping card {}: stored payload is not valid JSON", card.getId());
                continue;
            }
            docs.add(doc);
        }
        return docs;
    }

    /** CARD_GET: one card document by id, 404 when unknown. */
    @GetMapping("/cards/{id}")
    public ResponseEntity<JsonNode> getCard(@PathVariable String id) {
        Optional<Card> found = cardRepository.findById(id);
        if (found.isEmpty()) {
            return error(HttpStatus.NOT_FOUND, "unknown card id: " + id);
        }
        JsonNode doc = parseQuietly(found.get().getPayload());
        if (doc == null) {
            return error(HttpStatus.INTERNAL_SERVER_ERROR, "stored card document is corrupt");
        }
        return ResponseEntity.ok(doc);
    }

    /**
     * ROSTER_SAVE: accepts a {@code potato.roster/1} document. Rejects oversized
     * bodies, wrong schema tags, non-object bodies, a missing/non-array
     * {@code entries} field, and malformed entry elements with HTTP 400.
     */
    @PostMapping("/rosters")
    public ResponseEntity<JsonNode> saveRoster(@RequestBody byte[] body) {
        if (body.length > MAX_BODY_BYTES) {
            return error(HttpStatus.BAD_REQUEST, "body exceeds 2MB limit");
        }
        JsonNode root = parseQuietly(body);
        if (root == null || !root.isObject()) {
            return error(HttpStatus.BAD_REQUEST, "request body must be a JSON object");
        }
        JsonNode schema = root.get("schema");
        if (schema == null || !schema.isTextual() || !ROSTER_SCHEMA.equals(schema.asText())) {
            return error(HttpStatus.BAD_REQUEST, "schema must be \"" + ROSTER_SCHEMA + "\"");
        }
        JsonNode entries = root.get("entries");
        if (entries == null || !entries.isArray()) {
            return error(HttpStatus.BAD_REQUEST, "\"entries\" must be an array");
        }
        String violation = validateEntries(entries);
        if (violation != null) {
            return error(HttpStatus.BAD_REQUEST, violation);
        }

        // Canonical re-serialization: always valid UTF-8 regardless of the
        // charset Jackson auto-detected on the request body.
        Roster roster = new Roster(ROSTER_SCHEMA, entries.size(), Instant.now(), toJson(root));
        Roster saved = rosterRepository.save(roster);

        return ResponseEntity.status(HttpStatus.CREATED)
                .body(objectMapper.createObjectNode().put("id", saved.getId()));
    }

    /** Roster metadata list (id, entryCount, savedAt) — no payload bodies. */
    @GetMapping("/rosters")
    public List<RosterMeta> listRosters() {
        return rosterRepository.findAllBy().stream()
                .map(m -> new RosterMeta(m.getId(), m.getEntryCount(), m.getSavedAt()))
                .toList();
    }

    /** Full roster document by id, 404 when unknown. */
    @GetMapping("/rosters/{id}")
    public ResponseEntity<JsonNode> getRoster(@PathVariable long id) {
        Optional<Roster> found = rosterRepository.findById(id);
        if (found.isEmpty()) {
            return error(HttpStatus.NOT_FOUND, "unknown roster id: " + id);
        }
        JsonNode doc = parseQuietly(found.get().getPayload());
        if (doc == null) {
            return error(HttpStatus.INTERNAL_SERVER_ERROR, "stored roster document is corrupt");
        }
        return ResponseEntity.ok(doc);
    }

    /** ROSTER_DELETE: removes a stored roster by id, 404 when unknown. */
    @DeleteMapping("/rosters/{id}")
    public ResponseEntity<JsonNode> deleteRoster(@PathVariable long id) {
        if (!rosterRepository.existsById(id)) {
            return error(HttpStatus.NOT_FOUND, "unknown roster id: " + id);
        }
        rosterRepository.deleteById(id);
        return ResponseEntity.noContent().build();
    }

    /** Roster metadata projection for list responses. */
    public record RosterMeta(Long id, int entryCount, Instant savedAt) {
    }

    /**
     * Validates each {@code entries[]} element against the engine's writer:
     * required textual {@code name}/{@code rank}/{@code squad}, numeric
     * {@code team}, and — when present — boolean {@code alive}, numeric
     * {@code deathTime}, textual {@code relic}/{@code art}.
     *
     * @return a description of the first violation, or {@code null} when valid
     */
    private String validateEntries(JsonNode entries) {
        for (int i = 0; i < entries.size(); i++) {
            JsonNode e = entries.get(i);
            String at = "entries[" + i + "]";
            if (!e.isObject()) {
                return at + " must be an object";
            }
            for (String field : new String[] {"name", "rank", "squad"}) {
                JsonNode v = e.get(field);
                if (v == null || !v.isTextual()) {
                    return at + "." + field + " must be a string";
                }
            }
            JsonNode team = e.get("team");
            if (team == null || !team.isNumber()) {
                return at + ".team must be a number";
            }
            JsonNode alive = e.get("alive");
            if (alive != null && !alive.isBoolean()) {
                return at + ".alive must be a boolean";
            }
            JsonNode deathTime = e.get("deathTime");
            if (deathTime != null && !deathTime.isNumber()) {
                return at + ".deathTime must be a number";
            }
            for (String field : new String[] {"relic", "art"}) {
                JsonNode v = e.get(field);
                if (v != null && !v.isTextual()) {
                    return at + "." + field + " must be a string";
                }
            }
        }
        return null;
    }

    private JsonNode parseQuietly(byte[] bytes) {
        try {
            return objectMapper.readTree(bytes);
        } catch (Exception e) {
            return null;
        }
    }

    private JsonNode parseQuietly(String text) {
        try {
            return objectMapper.readTree(text);
        } catch (Exception e) {
            return null;
        }
    }

    private String toJson(JsonNode node) {
        try {
            return objectMapper.writeValueAsString(node);
        } catch (JsonProcessingException e) {
            throw new IllegalStateException("failed to serialize parsed document", e);
        }
    }

    private ResponseEntity<JsonNode> error(HttpStatus status, String message) {
        return ResponseEntity.status(status)
                .body(objectMapper.createObjectNode().put("error", message));
    }
}
