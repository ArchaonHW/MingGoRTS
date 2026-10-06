package com.potato.rts.roster;

import static org.assertj.core.api.Assertions.assertThat;

import com.potato.rts.roster.web.PayloadSizeLimitFilter;
import com.potato.rts.roster.web.RosterController;
import org.junit.jupiter.api.Test;
import org.springframework.mock.web.MockFilterChain;
import org.springframework.mock.web.MockHttpServletRequest;
import org.springframework.mock.web.MockHttpServletResponse;

class PayloadSizeLimitFilterTest {

    private final PayloadSizeLimitFilter filter = new PayloadSizeLimitFilter();

    @Test
    void declaredContentLengthOverCap_returns413WithoutInvokingChain() throws Exception {
        // Simulates a large declared Content-Length without buffering 2 MiB.
        MockHttpServletRequest request = new MockHttpServletRequest("POST", "/api/rosters") {
            @Override
            public long getContentLengthLong() {
                return RosterController.MAX_BODY_BYTES + 1L;
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
        MockHttpServletRequest request = new MockHttpServletRequest("POST", "/api/rosters");
        request.setContent("{}".getBytes());
        MockHttpServletResponse response = new MockHttpServletResponse();
        MockFilterChain chain = new MockFilterChain();

        filter.doFilter(request, response, chain);

        assertThat(chain.getRequest()).isSameAs(request);
        assertThat(response.getStatus()).isEqualTo(200);
    }
}
