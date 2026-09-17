package com.potato.rts.roster;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatCode;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.argThat;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import java.nio.charset.StandardCharsets;
import java.util.Optional;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.potato.rts.roster.model.Card;
import com.potato.rts.roster.repo.CardRepository;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;
import org.springframework.core.io.Resource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;

/**
 * The seeder resolves {@code classpath:/seed/*.json}, which in tests sees both
 * the bundled main seeds and the deliberately broken files under
 * {@code src/test/resources/seed/} (zzz_*).
 */
class CardSeederTest {

    private final CardRepository cardRepository = mock(CardRepository.class);
    private final ObjectMapper objectMapper = new ObjectMapper();
    private final CardSeeder seeder = new CardSeeder(cardRepository, objectMapper);

    private static Resource[] seedResources() throws Exception {
        return new PathMatchingResourcePatternResolver().getResources("classpath*:/seed/*.json");
    }

    /** Counts the seed resources the seeder is expected to accept. */
    private int expectedValidSeeds() throws Exception {
        int valid = 0;
        for (Resource r : seedResources()) {
            try {
                JsonNode root = objectMapper.readTree(r.getInputStream().readAllBytes());
                JsonNode id = root == null ? null : root.get("id");
                if (root != null && root.isObject()
                        && "potato.character_card/1".equals(root.path("schema").asText())
                        && id != null && id.isTextual() && !id.asText().isBlank()) {
                    valid++;
                }
            } catch (Exception ignored) {
                // malformed file — seeder must skip it too
            }
        }
        return valid;
    }

    @Test
    void run_savesEveryValidBundledSeed_andSkipsBrokenOnes() throws Exception {
        when(cardRepository.findById(anyString())).thenReturn(Optional.empty());

        assertThatCode(() -> seeder.run()).doesNotThrowAnyException();

        ArgumentCaptor<Card> captor = ArgumentCaptor.forClass(Card.class);
        verify(cardRepository, times(expectedValidSeeds())).save(captor.capture());
        assertThat(expectedValidSeeds()).isGreaterThanOrEqualTo(31);
        for (Card card : captor.getAllValues()) {
            assertThat(card.getId()).isNotBlank();
            assertThat(card.getSchemaVersion()).isEqualTo("potato.character_card/1");
            JsonNode doc = objectMapper.readTree(card.getPayload());
            assertThat(doc.path("id").asText()).isEqualTo(card.getId());
        }
    }

    @Test
    void run_skipsIdenticalExisting_updatesChangedExisting() throws Exception {
        String chiangPayload = seedPayload("roc_chiang_kaishek.json");
        Card identical = new Card("roc_chiang_kaishek", "old",
                "potato.character_card/1", chiangPayload);
        Card stale = new Card("roc_bai_chongxi", "old",
                "potato.character_card/1", "{\"stale\":true}");
        when(cardRepository.findById(anyString())).thenAnswer(inv -> {
            String id = inv.getArgument(0);
            if (id.equals("roc_chiang_kaishek")) {
                return Optional.of(identical);
            }
            if (id.equals("roc_bai_chongxi")) {
                return Optional.of(stale);
            }
            return Optional.empty();
        });

        seeder.run();

        // Identical payload -> left untouched.
        verify(cardRepository, never())
                .save(argThat(c -> "roc_chiang_kaishek".equals(c.getId())));
        // Changed payload -> the existing row is updated with the new document.
        verify(cardRepository).save(argThat(c -> "roc_bai_chongxi".equals(c.getId())
                && c.getPayload().contains("roc_bai_chongxi")));
    }

    private String seedPayload(String filename) throws Exception {
        Resource[] matches = new PathMatchingResourcePatternResolver()
                .getResources("classpath*:/seed/" + filename);
        return new String(matches[0].getInputStream().readAllBytes(), StandardCharsets.UTF_8);
    }
}
