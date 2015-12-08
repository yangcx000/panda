#include "panda_client.h"

#include <string.h>

void event_cb(struct bufferevent* bev, short events, void* ctx) {
    if (events & BEV_EVENT_CONNECTED) {
        fprintf(stderr, "Connect okay.\n");
    } else if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {
        struct event_base* evbase = (struct event_base*)ctx;
        if (events & BEV_EVENT_ERROR) {
            int err = bufferevent_socket_get_dns_error(bev);
            if (err)
                fprintf(stderr, "DNS error: %s\n", evutil_gai_strerror(err));
        }
        fprintf(stderr, "Error: couldn't connect server\n");
        bufferevent_free(bev);
        event_base_loopexit(evbase, NULL);
    }
}

void read_cb(struct bufferevent* bev, void* ctx) {
    char buf[1024];
    int n;
    struct evbuffer* input = bufferevent_get_input(bev);
    while ((n = evbuffer_remove(input, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, n, stdout);
    }
}

void write_cb(struct bufferevent* bev, void* ctx) {

}

int main(void) {
    struct event_base* evbase;

    evbase = event_base_new();
    if (!evbase) {
        fprintf(stderr, "Error: event_base_new failed\n");
        return 1;
    }

    struct bufferevent* bev = bufferevent_socket_new(evbase, -1, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        fprintf(stderr, "Error: bufferevent_socket_new failed\n");
        return 1;
    }

    bufferevent_setcb(bev, read_cb, write_cb, event_cb, evbase);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0); // connect 127.0.0.1
    sin.sin_port = htons(21000);

    // TODO: use bufferevent_socket_connect_hostname() simply
    if (bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        /* Error starting connection */
        fprintf(stderr, "Error: bufferevent_socket_connect failed\n");
        bufferevent_free(bev);
        return 1;
    }

    event_base_dispatch(evbase);
    return 0;
}
