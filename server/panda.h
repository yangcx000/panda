#ifndef ECHOSERVER_THREADED_H
#define ECHOSERVER_THREADED_H

//#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <unistd.h>

/* Port to listen on. */
#define SERVER_PORT 21000
#define SERVER_ADDR 127.0.0.1
/* Connection backlog (# of backlogged connections to accept). */
#define CONNECTION_BACKLOG 8
/* Socket read and write timeouts, in seconds. */
#define SOCKET_READ_TIMEOUT_SECONDS 10
#define SOCKET_WRITE_TIMEOUT_SECONDS 10
/* Number of worker threads.  Should match number of CPU cores reported in /proc/cpuinfo. */
#define NUM_THREADS 8

#define log(...) {\
    fprintf(stderr, "%s:%d: %s():\t", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, __VA_ARGS__); \
}

void listen_cb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *address, int socklen, void *ctx);
void listen_error_cb(struct evconnlistener *listener, void *ctx);

void read_cb(struct bufferevent *bev, void *ctx);
void write_cb(struct bufferevent *bev, void *ctx);
void event_cb(struct bufferevent *bev, short events, void *ctx);

void killServer(void);
void sighandler(int signal);

#endif
