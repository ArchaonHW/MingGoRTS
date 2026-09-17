package com.potato.rts.roster.repo;

import java.time.Instant;
import java.util.List;

import com.potato.rts.roster.model.Roster;
import org.springframework.data.jpa.repository.JpaRepository;

public interface RosterRepository extends JpaRepository<Roster, Long> {

    /**
     * Metadata-only view of a roster row — avoids loading the {@code @Lob}
     * payload just to build the list endpoint response.
     */
    interface Meta {
        Long getId();

        int getEntryCount();

        Instant getSavedAt();
    }

    /** All rosters as metadata projections (no payload column selected). */
    List<Meta> findAllBy();
}
