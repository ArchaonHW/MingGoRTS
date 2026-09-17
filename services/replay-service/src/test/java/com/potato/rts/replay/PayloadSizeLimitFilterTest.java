package com.potato.rts.replay;

import static org.assertj.core.api.Assertions.assertThat;

import com.potato.rts.replay.web.PayloadSizeLimitFilter;
import com.potato.rts.replay.web.ReplayController;
import org.junit.jupiter.api.Test;
import org.springframework.mock.web.MockFilterChain;
import org.springframework.mock.web.MockHttpServletRequest;
import org.springframework.mock.web.MockHttpServletResponse;

class PayloadSizeLimitFilterTest {

    private final PayloadSizeLimitFilter filter = new PayloadSizeLimitFilter();

    @Test
    void declaredContentLengthOverCap_returns413WithoutInvokingChain() throws Exception {
        // Simulates a large declared Content-Length without buffering 2 MiB.
        MockHttpServletRequest request = new MockHttpServletRequest("POST", "/api/replays") {
            @Override
            public long getContentLengthLong() {
                return ReplayController.MAX_BODY_BYTES + 1L;
            }
        };
        MockHttpServletResponse response = new MockHttpServletResponse();
        MockFilterChain chain = new MockFilterChain();

        filter.doFilter(request, response, chain);

        assertThat(response.getStatus()).isEqualTo(413);
        assertThat(response.getContentType()).startsWith("application/json");
        // The downstream chain was never invoked — no servlet was registered.
        assertThat(chain.getRequest()).isNull();
    }

    @Test
    void declaredContentLengthUnderCap_passesThrough() throws Exception {
        MockHttpServletRequest request = new MockHttpServletRequest("POST", "/api/replays");
        request.setContent("{}".getBytes());
        MockHttpServletResponse response = new MockHttpServletResponse();
        MockFilterChain chain = new MockFilterChain();

        filter.doFilter(request, response, chain);

        assertThat(chain.getRequest()).isSameAs(request);
        assertThat(response.getStatus()).isEqualTo(200);
    }
}
