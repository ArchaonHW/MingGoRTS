package com.potato.rts.roster;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/**
 * Roster service: owns character-card ({@code potato.character_card/1}) and
 * battle-roster ({@code potato.roster/1}) domains. Persists to a file-backed
 * embedded H2 database so data survives restarts.
 */
@SpringBootApplication
public class RosterServiceApplication {

    public static void main(String[] args) {
        SpringApplication.run(RosterServiceApplication.class, args);
    }
}
