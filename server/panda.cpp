#include "panda.h"

#include <arpa/inet.h>
#include <signal.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static struct event_base* evbase;

void killServer(void) {
	fprintf(stderr, "Stopping socket listener event loop.\n");
    if (event_base_loopexit(evbase, NULL)) {
		perror("Error shutting down server");
	}
	fprintf(stderr, "Stopping workers.\n");
//	workqueue_shutdown(&workqueue);
}

void sighandler(int signal) {
    fprintf(stderr, "Received signal %d: %s.  Shutting down.\n", signal, strsignal(signal));
	killServer();
}


//void closeClient(client_t *client) {
//	if (client != NULL) {
//		if (client->fd >= 0) {
//			close(client->fd);
//			client->fd = -1;
//		}
//	}
//}
//
//void closeAndFreeClient(client_t *client) {
//	if (client != NULL) {
//		closeClient(client);
//		if (client->buf_ev != NULL) {
//			bufferevent_free(client->buf_ev);
//			client->buf_ev = NULL;
//		}
//		if (client->evbase != NULL) {
//			event_base_free(client->evbase);
//			client->evbase = NULL;
//		}
//		if (client->output_buffer != NULL) {
//			evbuffer_free(client->output_buffer);
//			client->output_buffer = NULL;
//		}
//		free(client);
//	}
//}
//
///**
// * Called by libevent when there is data to read.
// */
//void buffered_on_read(struct bufferevent *bev, void *arg) {
//    client_t *client = (client_t *)arg;
//    char data[4096];
//    int nbytes;
//
//    /* Copy the data from the input buffer to the output buffer in 4096-byte chunks.
//     * There is a one-liner to do the whole thing in one shot, but the purpose of this server
//     * is to show actual real-world reading and writing of the input and output buffers,
//     * so we won't take that shortcut here. */
//    while ((nbytes = EVBUFFER_LENGTH(bev->input)) > 0) {
//        /* Remove a chunk of data from the input buffer, copying it into our local array (data). */
//        if (nbytes > 4096) nbytes = 4096;
//        evbuffer_remove(bev->input, data, nbytes); 
//        /* Add the chunk of data from our local array (data) to the client's output buffer. */
//        evbuffer_add(client->output_buffer, data, nbytes);
//    }
//
//    /* Send the results to the client.  This actually only queues the results for sending.
//     * Sending will occur asynchronously, handled by libevent. */
//    if (bufferevent_write_buffer(bev, client->output_buffer)) {
//        errorOut("Error sending data to client on fd %d\n", client->fd);
//        closeClient(client);
//    }
//}

/**
 * Called by libevent when the write buffer reaches 0.  We only
 * provide this because libevent expects it, but we don't use it.
 */
//void buffered_on_write(struct bufferevent *bev, void *arg) {
//}
//
///**
// * Called by libevent when there is an error on the underlying socket
// * descriptor.
// */
//void buffered_on_error(struct bufferevent *bev, short what, void *arg) {
//    closeClient((client_t *)arg);
//}
//
//static void server_job_function(struct job *job) {
//    client_t *client = (client_t *)job->user_data;
//
//    event_base_dispatch(client->evbase);
//    closeAndFreeClient(client);
//    free(job);
//}

void write_cb(struct bufferevent *bev, void *ctx) {

}

void event_cb(struct bufferevent *bev, short events, void *ctx) {

}

void read_cb(struct bufferevent *bev, void *ctx) {
    struct evbuffer *input = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(input);
    if (len) {
        fprintf(stderr, "%d\n", len);
    }
}

void listen_cb(struct evconnlistener* listener, evutil_socket_t fd,
        struct sockaddr *address, int socklen, void *ctx) {
    struct event_base* base = evconnlistener_get_base(listener);
    struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);


    //  workqueue *wq = (workqueue *)ctx;
  //  client *client;
  //  job_t *job;

  //  /* Create a client object. */
  //  if ((client = malloc(sizeof(*client))) == NULL) {
  //      log("failed to allocate memory for client state");
  //      return;
  //  }
  //  memset(client, 0, sizeof(*client));
  //  client->fd = fd;

  //  if ((client->output_buffer = evbuffer_new()) == NULL) {
  //      log("client output buffer allocation failed");
  //      closeAndFreeClient(client);
  //      return;
  //  }

  //  if ((client->evbase = event_base_new()) == NULL) {
  //      log("client event_base creation failed");
  //      closeAndFreeClient(client);
  //      return;
  //  }

  //  if ((client->buf_ev = bufferevent_new(client_fd, buffered_on_read, buffered_on_write, buffered_on_error, client)) == NULL) {
  //      log("client bufferevent creation failed");
  //      closeAndFreeClient(client);
  //      return;
  //  }

  //  bufferevent_base_set(client->evbase, client->buf_ev);
  //  bufferevent_settimeout(client->buf_ev, SOCKET_READ_TIMEOUT_SECONDS, SOCKET_WRITE_TIMEOUT_SECONDS);
  //  bufferevent_enable(client->buf_ev, EV_READ);
  //  /* Create a job object and add it to the work queue. */
  //  if ((job = malloc(sizeof(*job))) == NULL) {
  //      log("failed to allocate memory for job state");
  //      closeAndFreeClient(client);
  //      return;
  //  }

  //  job->job_function = server_job_function;
  //  job->user_data = client;

  //  workqueue_add_job(workqueue, job);
}

static void listen_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base* base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}

int main(void) {
    struct evconnlistener* listener;
    struct sockaddr_in sin;

    /* 
     * Set signal handlers
     * Less portable than sigaction
     */
    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);

    evbase = event_base_new();
    if (!evbase) {
        log("Error: event_base_new failed");        
        return 1;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    // FIXME: listen on 127.0.0.1
    sin.sin_addr.s_addr = htonl(0);
    sin.sin_port = htons(SERVER_PORT);

    listener = evconnlistener_new_bind(evbase, listen_cb, NULL, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,
            -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        log("Couldn't create listener");
        return 1;
    }

    evconnlistener_set_error_cb(listener, listen_error_cb);
    event_base_dispatch(evbase);

    return 0;
}
