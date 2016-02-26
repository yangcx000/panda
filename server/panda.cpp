#include "panda.h"

#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "config.h"
#include "INIReader.h"

static struct event_base* evbase;
static const std::string program_name = "panda";

void killServer(void) {
	fprintf(stderr, "Stopping socket listener event loop.\n");
    if (event_base_loopexit(evbase, NULL)) {
		perror("Error shutting down server");
	}
	fprintf(stderr, "Stopping workers.\n");
}

void sighandler(int signal) {
    fprintf(stderr, "Received signal %d: %s.  Shutting down.\n", signal, strsignal(signal));
	killServer();
}

void write_cb(struct bufferevent *bev, void *ctx) {
}

void event_cb(struct bufferevent *bev, short events, void *ctx) {
}

void read_cb(struct bufferevent *bev, void *ctx) {
    struct evbuffer *input = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(input);
    if (len) {
        fprintf(stderr, "%d\n", (int)len);
    }
}

void listen_cb(struct evconnlistener* listener, evutil_socket_t fd,
        struct sockaddr *address, int socklen, void *ctx) {
    struct event_base* base = evconnlistener_get_base(listener);
    struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}

void listen_error_cb(struct evconnlistener *listener, void *ctx) {
    struct event_base* base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}

int create_listen_ports(Config* config) {
    struct sockaddr_in sin;
    struct evconnlistener* listener;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(atoi(config->ip().c_str()));
    sin.sin_port = htons(config->port());

    listener = evconnlistener_new_bind(evbase, listen_cb, NULL, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,
            -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        fprintf(stderr, "Couldn't create listener");
        return 1;
    }
    evconnlistener_set_error_cb(listener, listen_error_cb);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "ERROR: invalid parameters\n");
        return 1;
    }

    Config* config = new Config(argv[1]);
    config->ParseConfig();

    /* 
     * Set signal handlers
     * Less portable than sigaction
     */
    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);

    evbase = event_base_new();
    if (!evbase) {
        fprintf(stderr, "Error: event_base_new failed");        
        return 1;
    }

    if (create_listen_ports(config)) {
        fprintf(stderr, "ERROR: create server failed\n");
        return 1;
    }

    event_base_dispatch(evbase);
    return 0;
}
