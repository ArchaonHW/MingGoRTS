package com.potato.rts.common.web;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Test;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;

class JsonSupportTest {

    private final ObjectMapper mapper = new ObjectMapper();

    @Test
    void parseQuietly_validBytes_returnsTree() {
        JsonNode n = JsonSupport.parseQuietly(mapper, "{\"a\":1}".getBytes());
        assertNotNull(n);
        assertEquals(1, n.get("a").asInt());
    }

    @Test
    void parseQuietly_brokenBytes_returnsNull() {
        assertNull(JsonSupport.parseQuietly(mapper, "{broken".getBytes()));
    }

    @Test
    void parseQuietly_validText_returnsTree() {
        JsonNode n = JsonSupport.parseQuietly(mapper, "[1,2]");
        assertNotNull(n);
        assertTrue(n.isArray());
    }

    @Test
    void parseQuietly_brokenText_returnsNull() {
        assertNull(JsonSupport.parseQuietly(mapper, "not json"));
    }

    @Test
    void toJson_roundTripsTree() {
        JsonNode n = JsonSupport.parseQuietly(mapper, "{\"a\":\"b\"}");
        assertEquals("{\"a\":\"b\"}", JsonSupport.toJson(mapper, n));
    }

    @Test
    void toJson_unserializablePojoNode_throwsIllegalState() {
        // POJONode 包一個序列化必炸的物件——writeValueAsString 走到才失敗,
        // 觸發 IllegalStateException 包裝路徑
        Object unserializable = new Object() {
            @SuppressWarnings("unused")
            public Object getBad() {
                throw new RuntimeException("boom");
            }
        };
        JsonNode bad = mapper.getNodeFactory().pojoNode(unserializable);
        assertThrows(IllegalStateException.class,
                () -> JsonSupport.toJson(mapper, bad));
    }

    @Test
    void error_buildsStatusAndJsonBody() {
        ResponseEntity<JsonNode> r =
                JsonSupport.error(mapper, HttpStatus.NOT_FOUND, "unknown id: 7");
        assertEquals(HttpStatus.NOT_FOUND, r.getStatusCode());
        assertNotNull(r.getBody());
        assertEquals("unknown id: 7", r.getBody().get("error").asText());
    }
}
