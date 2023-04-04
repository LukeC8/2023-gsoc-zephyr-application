/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>

#include "llhttp.h"
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_pkt.h>

#include "app_socket.h"

#define BIND_PORT 8080

typedef struct http_request_partial {
    char **headers;
    char url[2048];
    int header_cursor;
    int url_cursor;
    int status;
} http_request_partial_t;

llhttp_t parser;
llhttp_settings_t settings;
http_request_partial_t request;

static const char content_big[] = {
    #include "response_big.html.bin.inc"
};
static const char content_small[] = {
    #include "response_small.html.bin.inc"
};
static const char content_404[] = {
    #include "response_404.html.bin.inc"
};

const char* handle_http_request(http_request_partial_t *r) {
    if (strstr(r->url, "/big") != NULL || strcmp(r->url, "/ ") == 0)
        return content_big;

    if (strstr(r->url, "/small") != NULL)
        return content_small;

    return content_404;
}

int on_message_complete(llhttp_t *p) {
    request.url[request.url_cursor++] = '\0';
    request.status = 0;
    return 0;
}

int on_url(llhttp_t *p, const char *at, size_t length) {
    int cursor = request.url_cursor++;

    if (cursor > 2046) {
        return HPE_USER;
    }

    request.url[cursor] = *at;

    return 0;
}

void main(void)
{
    int ret, counter = 0;
    socket_server_t server;

    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server.addr.sin_port = htons(BIND_PORT);

    ret = tcp_socket_server_init(&server);

    if (ret == -1) {
        printf("TCP socket could not be initialized... %d", errno);
        exit(0);
    }

    printf("Single-threaded basic HTTP server waits for a connection on "
            "port %d...\n", BIND_PORT);

    llhttp_settings_init(&settings);

    settings.on_message_complete = on_message_complete;
    settings.on_url = on_url;

    while (1) {
        socket_client_t client;
        char addr_str[32];
        int req_state = 0;
        const char *response;
        size_t len;

        ret = socket_server_wait_client(&client, &server);

        if (ret < 0) {
            printf("Error in accept: %d - continuing\n", errno);
            continue;
        }

        inet_ntop(client.addr.sin_family, &client.addr.sin_addr,
                addr_str, sizeof(addr_str));
        printf("Connection #%d from %s\n", counter++, addr_str);

        request.url_cursor = 0;
        request.status = 1;
        llhttp_init(&parser, HTTP_REQUEST, &settings);

        while (1) {
            ssize_t r;
            char c;
            llhttp_errno_t err;

            r = recv(client.socket_num, &c, 1, 0);
            if (r == 0) {
                goto close_client;
            }

            if (r < 0) {
                if (errno == EAGAIN || errno == EINTR) {
                    continue;
                }

                printf("Got error %d when receiving from "
                        "socket\n", errno);
                goto close_client;
            }

            err = llhttp_execute(&parser, (const char *) &c, 1);

            if (err != HPE_OK) {
                printf("Parse error: %s %s\n", llhttp_errno_name(err),
                        parser.reason);
                goto close_client;
            }

            if (req_state == 0 && c == '\r') {
                req_state++;
            } else if (req_state == 1 && c == '\n') {
                req_state++;
            } else if (req_state == 2 && c == '\r') {
                req_state++;
            } else if (req_state == 3 && c == '\n') {
                break;
            } else {
                req_state = 0;
            }
        }

        response = handle_http_request(&request);
        len = strlen(response);

        while (len) {
            int sent_len = send(client.socket_num, response, len, 0);

            if (sent_len == -1) {
                printf("Error sending data to peer, errno: %d\n", errno);
                break;
            }
            response += sent_len;
            len -= sent_len;
        }

close_client:
        ret = close(client.socket_num);
        if (ret == 0) {
            printf("Connection from %s closed\n", addr_str);
        } else {
            printf("Got error %d while closing the "
                    "socket\n", errno);
        }

#if defined(__ZEPHYR__) && defined(CONFIG_NET_BUF_POOL_USAGE)
        struct k_mem_slab *rx, *tx;
        struct net_buf_pool *rx_data, *tx_data;

        net_pkt_get_info(&rx, &tx, &rx_data, &tx_data);
        printf("rx buf: %d, tx buf: %d\n",
                atomic_get(&rx_data->avail_count), atomic_get(&tx_data->avail_count));
#endif

    }
}
