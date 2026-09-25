package com.potato.rts.roster;

import static org.assertj.core.api.Assertions.assertThat;

import java.time.Instant;

import com.potato.rts.roster.model.Card;
import com.potato.rts.roster.model.Roster;
import com.potato.rts.roster.repo.CardRepository;
import com.potato.rts.roster.repo.RosterRepository;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;
import org.springframework.jdbc.core.JdbcTemplate;

/**
 * Flyway 遷移驗證：@DataJpaTest 起嵌入式 H2,Flyway 套用
 * db/migration/V1__init.sql,Hibernate ddl-auto=validate 核對 entity
 * 映射——兩者不一致時 context 直接起不來。
 */
@DataJpaTest
class SchemaMigrationTest {

    @Autowired
    private JdbcTemplate jdbc;

    @Autowired
    private CardRepository cardRepository;

    @Autowired
    private RosterRepository rosterRepository;

    @Test
    void flywayHistory_recordsV1() {
        // Flyway 建的表名是帶引號小寫——H2 大小寫敏感
        Integer applied = jdbc.queryForObject(
                "select count(*) from \"flyway_schema_history\" where \"success\" = true",
                Integer.class);
        assertThat(applied).isNotNull().isGreaterThanOrEqualTo(1);
    }

    @Test
    void cardTable_matchesEntity_roundTrips() {
        cardRepository.save(new Card("c1", "Tester", "potato.character_card/1",
                "{\"id\":\"c1\"}"));
        assertThat(cardRepository.findById("c1")).isPresent()
                .get().extracting(Card::getName).isEqualTo("Tester");
    }

    @Test
    void rosterTable_matchesEntity_roundTrips() {
        Roster saved = rosterRepository.save(new Roster("potato.roster/1", 2,
                Instant.parse("2026-09-25T00:00:00Z"), "{\"entries\":[]}"));
        assertThat(saved.getId()).isNotNull();
        assertThat(rosterRepository.findById(saved.getId())).isPresent();
    }
}
