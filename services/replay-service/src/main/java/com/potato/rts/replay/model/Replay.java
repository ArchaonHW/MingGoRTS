package com.potato.rts.replay.model;

import java.time.Instant;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Lob;
import jakarta.persistence.Table;

/**
 * An uploaded battle replay conforming to schema {@code potato.battle_replay/1}.
 * Events look like {@code {t, event}} and are stored verbatim inside
 * {@link #payload}; only metadata (id, eventCount, uploadedAt) is indexed.
 */
@Entity
@Table(name = "replays")
public class Replay {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String schemaVersion;

    private int eventCount;

    private Instant uploadedAt;

    /** Full replay JSON document, stored verbatim. */
    @Lob
    @Column(nullable = false)
    private String payload;

    protected Replay() {
        // JPA
    }

    public Replay(String schemaVersion, int eventCount, Instant uploadedAt, String payload) {
        this.schemaVersion = schemaVersion;
        this.eventCount = eventCount;
        this.uploadedAt = uploadedAt;
        this.payload = payload;
    }

    public Long getId() {
        return id;
    }

    public String getSchemaVersion() {
        return schemaVersion;
    }

    public void setSchemaVersion(String schemaVersion) {
        this.schemaVersion = schemaVersion;
    }

    public int getEventCount() {
        return eventCount;
    }

    public void setEventCount(int eventCount) {
        this.eventCount = eventCount;
    }

    public Instant getUploadedAt() {
        return uploadedAt;
    }

    public void setUploadedAt(Instant uploadedAt) {
        this.uploadedAt = uploadedAt;
    }

    public String getPayload() {
        return payload;
    }

    public void setPayload(String payload) {
        this.payload = payload;
    }
}
