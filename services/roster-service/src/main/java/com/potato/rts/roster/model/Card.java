package com.potato.rts.roster.model;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Lob;
import jakarta.persistence.Table;

/**
 * A character card document conforming to schema {@code potato.character_card/1}.
 * The full card JSON is stored verbatim in {@link #payload} so the service stays
 * a faithful consumer of the engine format rather than a new format authority.
 */
@Entity
@Table(name = "cards")
public class Card {

    /** Card id from the JSON document, e.g. "roc_chiang_kaishek". */
    @Id
    private String id;

    private String name;

    private String schemaVersion;

    /** Full card JSON document, stored verbatim. */
    @Lob
    @Column(nullable = false)
    private String payload;

    protected Card() {
        // JPA
    }

    public Card(String id, String name, String schemaVersion, String payload) {
        this.id = id;
        this.name = name;
        this.schemaVersion = schemaVersion;
        this.payload = payload;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSchemaVersion() {
        return schemaVersion;
    }

    public void setSchemaVersion(String schemaVersion) {
        this.schemaVersion = schemaVersion;
    }

    public String getPayload() {
        return payload;
    }

    public void setPayload(String payload) {
        this.payload = payload;
    }
}
