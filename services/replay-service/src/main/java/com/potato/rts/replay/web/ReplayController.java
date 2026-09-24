package com.potato.rts.replay.web;

import java.time.Instant;
import java.util.List;
import java.util.Optional;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.potato.rts.common.web.JsonSupport;
import com.potato.rts.replay.model.Replay;
import com.potato.rts.replay.repo.ReplayRepository;
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
 * REST API for battle replays ({@code potato.battle_replay/1}).
 *
 * <p>Uploads are validated before persisting: the {@code schema} tag must be
 * exactly {@code potato.battle_replay/1}, {@code events} must be an array of
 * well-formed elements, and the body must not exceed
 * {@value #MAX_BODY_BYTES} bytes. All response bodies are serialized through
 * Jackson so stored documents cannot corrupt the output.
 */
@RestController
@RequestMapping("/api/replays")
public class ReplayController {

    /** Only schema tag accepted for replay uploads. */
    static final String REPLAY_SCHEMA = "potato.battle_replay/1";

    /** Hard cap on upload size: 2 MiB. */
    public static final int MAX_BODY_BYTES = 2 * 1024 * 1024;

    private final ReplayRepository replayRepository;
    private final ObjectMapper objectMapper;

    public ReplayController(ReplayRepository replayRepository, ObjectMapper objectMapper) {
        this.replayRepository = replayRepository;
        this.objectMapper = objectMapper;
    }

    /**
     * REPLAY_UPLOAD: accepts a {@code potato.battle_replay/1} document and
     * returns 201 with the assigned id.
     */
    @PostMapping
    public ResponseEntity<JsonNode> upload(@RequestBody byte[] body) {
        if (body.length > MAX_BODY_BYTES) {
            return error(HttpStatus.BAD_REQUEST, "body exceeds 2MB limit");
        }
        JsonNode root = parseQuietly(body);
        if (root == null || !root.isObject()) {
            return error(HttpStatus.BAD_REQUEST, "request body must be a JSON object");
        }
        JsonNode schema = root.get("schema");
        if (schema == null || !schema.isTextual() || !REPLAY_SCHEMA.equals(schema.asText())) {
            return error(HttpStatus.BAD_REQUEST, "schema must be \"" + REPLAY_SCHEMA + "\"");
        }
        JsonNode events = root.get("events");
        if (events == null || !events.isArray()) {
            return error(HttpStatus.BAD_REQUEST, "\"events\" must be an array");
        }
        String violation = validateEvents(events);
        if (violation != null) {
            return error(HttpStatus.BAD_REQUEST, violation);
        }

        // Canonical re-serialization: always valid UTF-8 regardless of the
        // charset Jackson auto-detected on the request body.
        Replay replay = new Replay(REPLAY_SCHEMA, events.size(), Instant.now(),
                JsonSupport.toJson(objectMapper, root));
        Replay saved = replayRepository.save(replay);

        return ResponseEntity.status(HttpStatus.CREATED)
                .body(objectMapper.createObjectNode().put("id", saved.getId()));
    }

    /**
     * REPLAY_LIST: metadata only (id, eventCount, uploadedAt) — full event
     * bodies are served by {@link #getReplay(long)}.
     */
    @GetMapping
    public List<ReplayMeta> listReplays() {
        return replayRepository.findAllBy().stream()
                .map(m -> new ReplayMeta(m.getId(), m.getEventCount(), m.getUploadedAt()))
                .toList();
    }

    /** REPLAY_GET: full replay document by id, 404 when unknown. */
    @GetMapping("/{id}")
    public ResponseEntity<JsonNode> getReplay(@PathVariable long id) {
        Optional<Replay> found = replayRepository.findById(id);
        if (found.isEmpty()) {
            return error(HttpStatus.NOT_FOUND, "unknown replay id: " + id);
        }
        JsonNode doc = parseQuietly(found.get().getPayload());
        if (doc == null) {
            return error(HttpStatus.INTERNAL_SERVER_ERROR, "stored replay document is corrupt");
        }
        return ResponseEntity.ok(doc);
    }

    /** REPLAY_DELETE: removes a stored replay by id, 404 when unknown. */
    @DeleteMapping("/{id}")
    public ResponseEntity<JsonNode> deleteReplay(@PathVariable long id) {
        if (!replayRepository.existsById(id)) {
            return error(HttpStatus.NOT_FOUND, "unknown replay id: " + id);
        }
        replayRepository.deleteById(id);
        return ResponseEntity.noContent().build();
    }

    /** Replay metadata projection for list responses. */
    public record ReplayMeta(Long id, int eventCount, Instant uploadedAt) {
    }

    /**
     * Validates each {@code events[]} element against the engine's writer:
     * an object with numeric {@code t} and textual {@code event}.
     *
     * @return a description of the first violation, or {@code null} when valid
     */
    private String validateEvents(JsonNode events) {
        for (int i = 0; i < events.size(); i++) {
            JsonNode e = events.get(i);
            String at = "events[" + i + "]";
            if (!e.isObject()) {
                return at + " must be an object";
            }
            JsonNode t = e.get("t");
            if (t == null || !t.isNumber()) {
                return at + ".t must be a number";
            }
            JsonNode event = e.get("event");
            if (event == null || !event.isTextual()) {
                return at + ".event must be a string";
            }
        }
        return null;
    }

    private JsonNode parseQuietly(byte[] bytes) {
        return JsonSupport.parseQuietly(objectMapper, bytes);
    }

    private JsonNode parseQuietly(String text) {
        return JsonSupport.parseQuietly(objectMapper, text);
    }

    private ResponseEntity<JsonNode> error(HttpStatus status, String message) {
        return JsonSupport.error(objectMapper, status, message);
    }
}
