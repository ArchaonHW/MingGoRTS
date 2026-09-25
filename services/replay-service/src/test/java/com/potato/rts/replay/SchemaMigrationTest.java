package com.potato.rts.replay;

import static org.assertj.core.api.Assertions.assertThat;

import java.time.Instant;

import com.potato.rts.replay.model.Replay;
import com.potato.rts.replay.repo.ReplayRepository;
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
    private ReplayRepository replayRepository;

    @Test
    void flywayHistory_recordsV1() {
        // Flyway 建的表名是帶引號小寫——H2 大小寫敏感
        Integer applied = jdbc.queryForObject(
                "select count(*) from \"flyway_schema_history\" where \"success\" = true",
                Integer.class);
        assertThat(applied).isNotNull().isGreaterThanOrEqualTo(1);
    }

    @Test
    void replayTable_matchesEntity_roundTrips() {
        Replay saved = replayRepository.save(new Replay("potato.battle_replay/1", 3,
                Instant.parse("2026-09-25T00:00:00Z"), "{\"events\":[]}"));
        assertThat(saved.getId()).isNotNull();
        assertThat(replayRepository.findById(saved.getId())).isPresent()
                .get().extracting(Replay::getEventCount).isEqualTo(3);
    }
}
