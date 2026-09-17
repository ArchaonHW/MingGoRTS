package com.potato.rts.gateway;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/**
 * API gateway. Single entry point for clients; routes are resolved through
 * Eureka discovery ({@code lb://} URIs) so instances can come and go freely.
 */
@SpringBootApplication
public class ApiGatewayApplication {

    public static void main(String[] args) {
        SpringApplication.run(ApiGatewayApplication.class, args);
    }
}
