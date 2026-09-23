package com.potato.rts.replay;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.delete;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import java.time.Instant;
import java.util.List;
import java.util.Optional;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.potato.rts.replay.model.Replay;
import com.potato.rts.replay.repo.ReplayRepository;
import com.potato.rts.replay.web.ReplayController;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.test.context.bean.override.mockito.MockitoBean;
import org.springframework.test.util.ReflectionTestUtils;
import org.springframework.test.web.servlet.MockMvc;

@WebMvcTest(ReplayController.class)
class ReplayControllerTest {

    private static final String REPLAY_JSON =
            "{\"schema\":\"potato.battle_replay/1\",\"events\":[{\"t\":1.0,\"event\":\"test\"}]}";

    @Autowired
    private MockMvc mockMvc;

    @MockitoBean
    private ReplayRepository replayRepository;

    private static Replay replay(long id, String payload, int eventCount) {
        Replay r = new Replay("potato.battle_replay/1", eventCount,
                Instant.parse("2026-09-17T00:00:00Z"), payload);
        ReflectionTestUtils.setField(r, "id", id);
        return r;
    }

    private static ReplayRepository.Meta meta(long id, int eventCount) {
        return new ReplayRepository.Meta() {
            @Override
            public Long getId() {
                return id;
            }

            @Override
            public int getEventCount() {
                return eventCount;
            }

            @Override
            public Instant getUploadedAt() {
                return Instant.parse("2026-09-17T00:00:00Z");
            }
        };
    }

    @Test
    void upload_validBody_returns201WithId() throws Exception {
        when(replayRepository.save(any(Replay.class))).thenAnswer(inv -> {
            Replay r = inv.getArgument(0);
            ReflectionTestUtils.setField(r, "id", 1L);
            return r;
        });

        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(REPLAY_JSON))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.id").value(1));
    }

    @Test
    void upload_wrongSchema_returns400() throws Exception {
        // Mimics `curl -d` which posts form-urlencoded — still must be 400, not 415.
        mockMvc.perform(post("/api/replays")
                        .content("{\"schema\":\"wrong/1\"}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_missingEvents_returns400() throws Exception {
        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.battle_replay/1\"}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_eventsNotArray_returns400() throws Exception {
        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.battle_replay/1\",\"events\":{}}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_eventElementMissingT_returns400() throws Exception {
        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.battle_replay/1\",\"events\":[{\"event\":\"x\"}]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_eventElementNonTextualEvent_returns400() throws Exception {
        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.battle_replay/1\",\"events\":[{\"t\":1.0,\"event\":3}]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_eventElementNotObject_returns400() throws Exception {
        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.battle_replay/1\",\"events\":[\"x\"]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_duplicateKeys_returns400() throws Exception {
        // STRICT_DUPLICATE_DETECTION must reject docs with two "schema" fields.
        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"wrong/1\",\"schema\":\"potato.battle_replay/1\",\"events\":[]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void upload_bodyOver2MB_isRejected() throws Exception {
        byte[] big = new byte[ReplayController.MAX_BODY_BYTES + 1];
        big[0] = '{';
        big[big.length - 1] = '}';

        mockMvc.perform(post("/api/replays")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(big))
                .andExpect(status().isPayloadTooLarge());
    }

    @Test
    void upload_bodyOver2MBDirectCall_returns400FromController() {
        // Chunked/no-length requests bypass the filter; the controller cap still applies.
        ReplayController controller = new ReplayController(replayRepository, new ObjectMapper());
        byte[] big = new byte[ReplayController.MAX_BODY_BYTES + 1];

        assertThat(controller.upload(big).getStatusCode()).isEqualTo(HttpStatus.BAD_REQUEST);
    }

    @Test
    void listReplays_returnsMetadataOnly() throws Exception {
        when(replayRepository.findAllBy()).thenReturn(List.of(meta(1L, 1)));

        mockMvc.perform(get("/api/replays"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].id").value(1))
                .andExpect(jsonPath("$[0].eventCount").value(1))
                .andExpect(jsonPath("$[0].uploadedAt").exists())
                .andExpect(jsonPath("$[0].payload").doesNotExist())
                .andExpect(jsonPath("$[0].events").doesNotExist());
    }

    @Test
    void getReplay_knownId_returnsFullDocument() throws Exception {
        when(replayRepository.findById(1L)).thenReturn(Optional.of(replay(1L, REPLAY_JSON, 1)));

        mockMvc.perform(get("/api/replays/1"))
                .andExpect(status().isOk())
                .andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
                .andExpect(jsonPath("$.schema").value("potato.battle_replay/1"))
                .andExpect(jsonPath("$.events[0].event").value("test"));
    }

    @Test
    void getReplay_unknownId_returns404() throws Exception {
        when(replayRepository.findById(99L)).thenReturn(Optional.empty());

        mockMvc.perform(get("/api/replays/99"))
                .andExpect(status().isNotFound());
    }

    @Test
    void getReplay_corruptStoredPayload_returns500() throws Exception {
        when(replayRepository.findById(5L)).thenReturn(Optional.of(replay(5L, "{broken", 0)));

        mockMvc.perform(get("/api/replays/5"))
                .andExpect(status().isInternalServerError());
    }

    @Test
    void deleteReplay_knownId_returns204AndDeletes() throws Exception {
        when(replayRepository.existsById(1L)).thenReturn(true);

        mockMvc.perform(delete("/api/replays/1"))
                .andExpect(status().isNoContent());
        verify(replayRepository).deleteById(1L);
    }

    @Test
    void deleteReplay_unknownId_returns404() throws Exception {
        when(replayRepository.existsById(99L)).thenReturn(false);

        mockMvc.perform(delete("/api/replays/99"))
                .andExpect(status().isNotFound());
    }
}
