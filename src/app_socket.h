/*
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <zephyr/net/socket.h>

typedef struct socket_server {
    struct sockaddr_in addr;
    int max_connections;
    int socket_num;
} socket_server_t;

typedef struct socket_client {
    struct sockaddr_in addr;
    int socket_num;
} socket_client_t;

int tcp_socket_server_init(socket_server_t* serv);

int socket_server_wait_client(socket_client_t *new_client,
                                            socket_server_t *srv);
#endif
