package com.potato.rts.gateway;

import static org.assertj.core.api.Assertions.assertThat;

import java.net.ConnectException;
import java.net.UnknownHostException;

import io.netty.handler.timeout.ReadTimeoutException;
import org.junit.jupiter.api.Test;
import org.springframework.cloud.gateway.support.NotFoundException;
import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.mock.http.server.reactive.MockServerHttpRequest;
import org.springframework.mock.web.server.MockServerWebExchange;
import org.springframework.web.server.ResponseStatusException;
import reactor.netty.http.client.PrematureCloseException;

class GatewayErrorHandlerTest {

    private final GatewayErrorHandler handler = new GatewayErrorHandler();

    private MockServerWebExchange exchange() {
        return MockServerWebExchange.from(MockServerHttpRequest.get("/api/cards").build());
    }

    private int statusFor(Throwable ex) {
        MockServerWebExchange exchange = exchange();
        handler.handle(exchange, ex).block();
        HttpStatusCode status = exchange.getResponse().getStatusCode();
        assertThat(status).isNotNull();
        return status.value();
    }

    @Test
    void connectException_mapsTo503() {
        assertThat(statusFor(new ConnectException("refused"))).isEqualTo(503);
    }

    @Test
    void notFoundException_mapsTo503() {
        assertThat(statusFor(new NotFoundException("Unable to find instance for lb://roster-service")))
                .isEqualTo(503);
    }

    @Test
    void upstreamFailureWrappedInOtherExceptions_stillMapsTo503() {
        assertThat(statusFor(new RuntimeException("wrap", new ConnectException("refused"))))
                .isEqualTo(503);
    }

    @Test
    void unknownHostException_mapsTo503() {
        assertThat(statusFor(new UnknownHostException("roster-service"))).isEqualTo(503);
    }

    @Test
    void nettyReadTimeout_mapsTo503() {
        assertThat(statusFor(ReadTimeoutException.INSTANCE)).isEqualTo(503);
    }

    @Test
    void prematureClose_mapsTo503() {
        assertThat(statusFor(PrematureCloseException.TEST_EXCEPTION)).isEqualTo(503);
    }

    @Test
    void responseStatusException_passesThrough() {
        assertThat(statusFor(new ResponseStatusException(HttpStatus.BAD_GATEWAY))).isEqualTo(502);
    }

    @Test
    void responseStatusExceptionWithNonStandardCode_doesNotThrow() {
        // HttpStatus.valueOf would throw on 599; the handler must not.
        assertThat(statusFor(new ResponseStatusException(HttpStatusCode.valueOf(599))))
                .isEqualTo(599);
    }

    @Test
    void unrelatedException_mapsTo500() {
        assertThat(statusFor(new IllegalStateException("bug"))).isEqualTo(500);
    }
}
