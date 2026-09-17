package com.potato.rts.replay;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/**
 * Replay service: owns the battle-replay domain ({@code potato.battle_replay/1}).
 * Persists to a file-backed embedded H2 database so uploaded replays survive
 * restarts.
 */
@SpringBootApplication
public class ReplayServiceApplication {

    public static void main(String[] args) {
        SpringApplication.run(ReplayServiceApplication.class, args);
    }
}
