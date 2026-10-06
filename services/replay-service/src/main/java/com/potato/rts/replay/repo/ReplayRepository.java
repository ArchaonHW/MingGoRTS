package com.potato.rts.replay.repo;

import java.time.Instant;
import java.util.List;

import com.potato.rts.replay.model.Replay;
import org.springframework.data.jpa.repository.JpaRepository;

public interface ReplayRepository extends JpaRepository<Replay, Long> {

    /**
     * Metadata-only view of a replay row — avoids loading the {@code @Lob}
     * payload just to build the list endpoint response.
     */
    interface Meta {
        Long getId();

        int getEventCount();

        Instant getUploadedAt();
    }

    /** All replays as metadata projections (no payload column selected). */
    List<Meta> findAllBy();
}
