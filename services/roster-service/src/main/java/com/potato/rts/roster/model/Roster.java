package com.potato.rts.roster.model;

import java.time.Instant;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Lob;
import jakarta.persistence.Table;

/**
 * A saved battle roster conforming to schema {@code potato.roster/1}.
 * Entries look like {@code {name, rank, squad, team, alive, deathTime, relic, art}};
 * they are stored verbatim inside {@link #payload}.
 */
@Entity
@Table(name = "rosters")
public class Roster {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String schemaVersion;

    private int entryCount;

    private Instant savedAt;

    /** Full roster JSON document, stored verbatim. */
    @Lob
    @Column(nullable = false)
    private String payload;

    protected Roster() {
        // JPA
    }

    public Roster(String schemaVersion, int entryCount, Instant savedAt, String payload) {
        this.schemaVersion = schemaVersion;
        this.entryCount = entryCount;
        this.savedAt = savedAt;
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

    public int getEntryCount() {
        return entryCount;
    }

    public void setEntryCount(int entryCount) {
        this.entryCount = entryCount;
    }

    public Instant getSavedAt() {
        return savedAt;
    }

    public void setSavedAt(Instant savedAt) {
        this.savedAt = savedAt;
    }

    public String getPayload() {
        return payload;
    }

    public void setPayload(String payload) {
        this.payload = payload;
    }
}
