package com.potato.rts.roster;

import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Optional;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.potato.rts.roster.model.Card;
import com.potato.rts.roster.repo.CardRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.io.Resource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.stereotype.Component;

/**
 * Seeds the card catalog from the bundled {@code classpath:/seed/*.json} files
 * (verbatim copies of the engine's {@code assets/cards} documents).
 *
 * <p>Every seed file is processed in isolation: a malformed file or a racing
 * insert is logged and skipped, never fatal to startup. When a seed document
 * has changed since it was last stored, the existing row is updated so edited
 * seed files propagate on restart; identical rows are left untouched.
 */
@Component
public class CardSeeder implements CommandLineRunner {

    private static final Logger log = LoggerFactory.getLogger(CardSeeder.class);
    private static final String CARD_SCHEMA = "potato.character_card/1";

    private final CardRepository cardRepository;
    private final ObjectMapper objectMapper;

    public CardSeeder(CardRepository cardRepository, ObjectMapper objectMapper) {
        this.cardRepository = cardRepository;
        this.objectMapper = objectMapper;
    }

    @Override
    public void run(String... args) {
        Resource[] seeds;
        try {
            // classpath*: aggregates seed/ dirs from every classpath root
            // (test classpath adds extra entries alongside the bundled ones).
            seeds = new PathMatchingResourcePatternResolver().getResources("classpath*:/seed/*.json");
        } catch (Exception e) {
            log.error("Could not enumerate seed resources: {}", e.getMessage());
            return;
        }

        int seeded = 0;
        int updated = 0;
        for (Resource seed : seeds) {
            try (InputStream in = seed.getInputStream()) {
                String payload = new String(in.readAllBytes(), StandardCharsets.UTF_8);
                JsonNode root = objectMapper.readTree(payload);
                JsonNode schema = root == null ? null : root.get("schema");
                JsonNode id = root == null ? null : root.get("id");
                if (schema == null || !CARD_SCHEMA.equals(schema.asText())
                        || id == null || !id.isTextual() || id.asText().isBlank()) {
                    log.warn("Skipping seed file {}: not a {} document with a non-blank id",
                            seed.getFilename(), CARD_SCHEMA);
                    continue;
                }

                String cardId = id.asText();
                String name = root.path("name").asText("");
                Optional<Card> existing = cardRepository.findById(cardId);
                if (existing.isPresent()) {
                    Card card = existing.get();
                    if (!payload.equals(card.getPayload())) {
                        card.setName(name);
                        card.setSchemaVersion(CARD_SCHEMA);
                        card.setPayload(payload);
                        if (save(seed, card)) {
                            updated++;
                        }
                    }
                    continue;
                }
                if (save(seed, new Card(cardId, name, CARD_SCHEMA, payload))) {
                    seeded++;
                }
            } catch (Exception e) {
                // One bad seed file must not abort startup.
                log.warn("Skipping seed file {}: {}", seed.getFilename(), e.getMessage());
            }
        }
        log.info("Card seeding complete: {} new, {} updated, {} total",
                seeded, updated, cardRepository.count());
    }

    private boolean save(Resource seed, Card card) {
        try {
            cardRepository.save(card);
            return true;
        } catch (DataIntegrityViolationException e) {
            // A second instance may have inserted the same id concurrently.
            log.warn("Seed {} lost a concurrent insert race for id {}",
                    seed.getFilename(), card.getId());
            return false;
        }
    }
}
