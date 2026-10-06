package com.potato.rts.roster;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import java.time.Instant;
import java.util.List;
import java.util.Optional;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.potato.rts.roster.model.Card;
import com.potato.rts.roster.model.Roster;
import com.potato.rts.roster.repo.CardRepository;
import com.potato.rts.roster.repo.RosterRepository;
import com.potato.rts.roster.web.RosterController;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.test.context.bean.override.mockito.MockitoBean;
import org.springframework.test.util.ReflectionTestUtils;
import org.springframework.test.web.servlet.MockMvc;

@WebMvcTest(RosterController.class)
class RosterControllerTest {

    private static final String CARD_JSON =
            "{\"schema\":\"potato.character_card/1\",\"id\":\"roc_chiang_kaishek\",\"name\":\"Chiang\"}";

    private static final String ROSTER_JSON =
            "{\"schema\":\"potato.roster/1\",\"entries\":[{"
            + "\"name\":\"Squad A\",\"rank\":\"lt\",\"squad\":\"1st\",\"team\":0,"
            + "\"alive\":true,\"deathTime\":-1.0,\"relic\":\"\",\"art\":\"\"}]}";

    @Autowired
    private MockMvc mockMvc;

    @MockitoBean
    private CardRepository cardRepository;

    @MockitoBean
    private RosterRepository rosterRepository;

    private static Roster roster(long id, String payload, int entryCount) {
        Roster r = new Roster("potato.roster/1", entryCount,
                Instant.parse("2026-09-17T00:00:00Z"), payload);
        ReflectionTestUtils.setField(r, "id", id);
        return r;
    }

    private static RosterRepository.Meta meta(long id, int entryCount) {
        return new RosterRepository.Meta() {
            @Override
            public Long getId() {
                return id;
            }

            @Override
            public int getEntryCount() {
                return entryCount;
            }

            @Override
            public Instant getSavedAt() {
                return Instant.parse("2026-09-17T00:00:00Z");
            }
        };
    }

    @Test
    void listCards_returnsArray() throws Exception {
        when(cardRepository.findAll()).thenReturn(List.of(
                new Card("roc_chiang_kaishek", "Chiang", "potato.character_card/1", CARD_JSON)));

        mockMvc.perform(get("/api/cards"))
                .andExpect(status().isOk())
                .andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
                .andExpect(jsonPath("$[0].schema").value("potato.character_card/1"))
                .andExpect(jsonPath("$[0].id").value("roc_chiang_kaishek"));
    }

    @Test
    void listCards_corruptStoredPayload_isSkipped() throws Exception {
        when(cardRepository.findAll()).thenReturn(List.of(
                new Card("bad", "Bad", "potato.character_card/1", "{broken"),
                new Card("roc_chiang_kaishek", "Chiang", "potato.character_card/1", CARD_JSON)));

        mockMvc.perform(get("/api/cards"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.length()").value(1))
                .andExpect(jsonPath("$[0].id").value("roc_chiang_kaishek"));
    }

    @Test
    void getCard_knownId_returnsCard() throws Exception {
        when(cardRepository.findById("roc_chiang_kaishek")).thenReturn(Optional.of(
                new Card("roc_chiang_kaishek", "Chiang", "potato.character_card/1", CARD_JSON)));

        mockMvc.perform(get("/api/cards/roc_chiang_kaishek"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value("roc_chiang_kaishek"));
    }

    @Test
    void getCard_unknownId_returns404() throws Exception {
        when(cardRepository.findById("nope")).thenReturn(Optional.empty());

        mockMvc.perform(get("/api/cards/nope"))
                .andExpect(status().isNotFound());
    }

    @Test
    void saveRoster_validBody_returns201WithId() throws Exception {
        when(rosterRepository.save(any(Roster.class))).thenAnswer(inv -> {
            Roster r = inv.getArgument(0);
            ReflectionTestUtils.setField(r, "id", 1L);
            return r;
        });

        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(ROSTER_JSON))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.id").value(1));
    }

    @Test
    void saveRoster_wrongSchema_returns400() throws Exception {
        // Mimics `curl -d` which posts form-urlencoded — still must be 400, not 415.
        mockMvc.perform(post("/api/rosters")
                        .content("{\"schema\":\"wrong/1\",\"entries\":[]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_entriesNotArray_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.roster/1\",\"entries\":{}}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_missingEntries_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.roster/1\"}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_malformedJson_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{not json"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_entryElementMissingName_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.roster/1\",\"entries\":["
                                + "{\"rank\":\"lt\",\"squad\":\"1st\",\"team\":0}]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_entryElementNonNumericTeam_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.roster/1\",\"entries\":["
                                + "{\"name\":\"A\",\"rank\":\"lt\",\"squad\":\"1st\",\"team\":\"x\"}]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_entryElementNotObject_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.roster/1\",\"entries\":[\"x\"]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_entryOptionalFieldWrongType_returns400() throws Exception {
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"potato.roster/1\",\"entries\":["
                                + "{\"name\":\"A\",\"rank\":\"lt\",\"squad\":\"1st\",\"team\":0,"
                                + "\"alive\":\"yes\"}]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_duplicateKeys_returns400() throws Exception {
        // STRICT_DUPLICATE_DETECTION must reject docs with two "schema" fields.
        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"schema\":\"wrong/1\",\"schema\":\"potato.roster/1\",\"entries\":[]}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void saveRoster_bodyOver2MB_isRejected() throws Exception {
        byte[] big = new byte[RosterController.MAX_BODY_BYTES + 1];
        big[0] = '{';
        big[big.length - 1] = '}';

        mockMvc.perform(post("/api/rosters")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(big))
                .andExpect(status().isPayloadTooLarge());
    }

    @Test
    void saveRoster_bodyOver2MBDirectCall_returns400FromController() {
        // Chunked/no-length requests bypass the filter; the controller cap still applies.
        RosterController controller =
                new RosterController(cardRepository, rosterRepository, new ObjectMapper());
        byte[] big = new byte[RosterController.MAX_BODY_BYTES + 1];

        assertThat(controller.saveRoster(big).getStatusCode()).isEqualTo(HttpStatus.BAD_REQUEST);
    }

    @Test
    void listRosters_returnsMetadataOnly() throws Exception {
        when(rosterRepository.findAllBy()).thenReturn(List.of(meta(1L, 1)));

        mockMvc.perform(get("/api/rosters"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].id").value(1))
                .andExpect(jsonPath("$[0].entryCount").value(1))
                .andExpect(jsonPath("$[0].savedAt").exists())
                .andExpect(jsonPath("$[0].payload").doesNotExist())
                .andExpect(jsonPath("$[0].entries").doesNotExist());
    }

    @Test
    void getRoster_knownId_returnsFullDocument() throws Exception {
        when(rosterRepository.findById(1L)).thenReturn(Optional.of(roster(1L, ROSTER_JSON, 1)));

        mockMvc.perform(get("/api/rosters/1"))
                .andExpect(status().isOk())
                .andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
                .andExpect(jsonPath("$.schema").value("potato.roster/1"))
                .andExpect(jsonPath("$.entries[0].name").value("Squad A"));
    }

    @Test
    void getRoster_unknownId_returns404() throws Exception {
        when(rosterRepository.findById(99L)).thenReturn(Optional.empty());

        mockMvc.perform(get("/api/rosters/99"))
                .andExpect(status().isNotFound());
    }
}
