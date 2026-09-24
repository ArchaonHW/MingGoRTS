package com.potato.rts.common.web;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;

/**
 * Shared JSON helpers for service controllers.
 *
 * <p>Centralizes the parse/serialize/error-body boilerplate that was
 * duplicated across {@code ReplayController} and {@code RosterController}:
 * quiet parsing (null on failure, never throws), canonical re-serialization,
 * and uniform {@code {"error": ...}} response bodies.
 */
public final class JsonSupport {

    private JsonSupport() {
    }

    /** Parses bytes to a tree; returns {@code null} on any failure. */
    public static JsonNode parseQuietly(ObjectMapper mapper, byte[] bytes) {
        try {
            return mapper.readTree(bytes);
        } catch (Exception e) {
            return null;
        }
    }

    /** Parses text to a tree; returns {@code null} on any failure. */
    public static JsonNode parseQuietly(ObjectMapper mapper, String text) {
        try {
            return mapper.readTree(text);
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * Serializes a parsed tree back to text. A {@code JsonNode} that came from
     * {@link ObjectMapper#readTree} always serializes, so a failure here is a
     * bug, not bad input — surfaced as {@link IllegalStateException}.
     */
    public static String toJson(ObjectMapper mapper, JsonNode node) {
        try {
            return mapper.writeValueAsString(node);
        } catch (JsonProcessingException e) {
            throw new IllegalStateException("failed to serialize parsed document", e);
        }
    }

    /** Uniform error body: {@code {"error": "<message>"}}. */
    public static ResponseEntity<JsonNode> error(ObjectMapper mapper,
                                                 HttpStatus status,
                                                 String message) {
        return ResponseEntity.status(status)
                .body(mapper.createObjectNode().put("error", message));
    }
}
