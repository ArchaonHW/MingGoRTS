package com.potato.rts.gateway;

import java.net.ConnectException;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;
import java.util.Set;
import java.util.concurrent.TimeoutException;

import org.springframework.cloud.gateway.support.NotFoundException;
import org.springframework.core.annotation.Order;
import org.springframework.core.io.buffer.DataBuffer;
import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.http.MediaType;
import org.springframework.http.server.reactive.ServerHttpResponse;
import org.springframework.stereotype.Component;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.web.server.ServerWebExchange;
import reactor.core.publisher.Mono;

import org.springframework.boot.web.reactive.error.ErrorWebExceptionHandler;

/**
 * Gateway error rendering.
 *
 * <p>When a {@code lb://} route has no live instance the load balancer throws
 * {@link NotFoundException} ("Unable to find instance"); refused connections,
 * DNS failures, read timeouts, and dropped upstream connections surface as
 * {@link ConnectException}, {@link UnknownHostException}, Netty
 * {@code ReadTimeoutException}, or Reactor Netty {@code PrematureCloseException}.
 * All mean "the upstream service is down", so all are mapped to HTTP 503 per
 * the service contract — the default handler would emit 500 for them.
 */
@Order(-2)
@Component
public class GatewayErrorHandler implements ErrorWebExceptionHandler {

    /**
     * Transport-layer failures matched by class name so this handler keeps
     * working even if the exact reactor-netty/netty classes move packages.
     */
    private static final Set<String> UNAVAILABLE_CLASS_NAMES = Set.of(
            "io.netty.handler.timeout.ReadTimeoutException",
            "reactor.netty.http.client.PrematureCloseException");

    @Override
    public Mono<Void> handle(ServerWebExchange exchange, Throwable ex) {
        HttpStatusCode status = resolveStatus(ex);
        ServerHttpResponse response = exchange.getResponse();
        if (response.isCommitted()) {
            return Mono.error(ex);
        }
        response.setStatusCode(status);
        response.getHeaders().setContentType(MediaType.APPLICATION_JSON);
        String message = status.value() == HttpStatus.SERVICE_UNAVAILABLE.value()
                ? "upstream service unavailable"
                : "gateway error";
        byte[] bytes = ("{\"error\":\"" + message + "\"}").getBytes(StandardCharsets.UTF_8);
        DataBuffer buffer = response.bufferFactory().wrap(bytes);
        return response.writeWith(Mono.just(buffer));
    }

    private HttpStatusCode resolveStatus(Throwable ex) {
        for (Throwable t = ex; t != null; t = t.getCause()) {
            if (isUpstreamUnavailable(t)) {
                return HttpStatus.SERVICE_UNAVAILABLE;
            }
            if (t instanceof ResponseStatusException rse) {
                // Already an HttpStatusCode — passes through verbatim, including
                // codes that HttpStatus.valueOf() would reject with an exception.
                return rse.getStatusCode();
            }
        }
        return HttpStatus.INTERNAL_SERVER_ERROR;
    }

    private boolean isUpstreamUnavailable(Throwable t) {
        return t instanceof NotFoundException
                || t instanceof ConnectException
                || t instanceof TimeoutException
                || t instanceof UnknownHostException
                || UNAVAILABLE_CLASS_NAMES.contains(t.getClass().getName());
    }
}
