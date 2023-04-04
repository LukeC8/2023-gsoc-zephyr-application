/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_pkt.h>

#include "app_socket.h"

int tcp_socket_server_init(socket_server_t *serv) {
    int ret;

    serv->addr.sin_family = AF_INET;
    ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(ret == -1) {
        printf("%s:%d error %d\n\r", __func__, __LINE__, errno);
        exit(1);
    }

    serv->socket_num = ret;

    ret = bind(serv->socket_num, (struct sockaddr *) &serv->addr, sizeof(serv->addr));

    if (ret == -1) {
        printf("%s:%d error %d\n\r", __func__, __LINE__, errno);
        exit(1);
    }

    return listen(serv->socket_num, serv->max_connections);
}

int socket_server_wait_client(socket_client_t *new_client,
                                            socket_server_t *srv) {
    int ret;
    socklen_t client_addr_len = sizeof(new_client->addr);

    ret = accept(srv->socket_num, (struct sockaddr *) &new_client->addr,
            &client_addr_len);

    if (ret < 0) {
        printf("Error in client accept - %d\n\r", errno);
    }

    new_client->socket_num = ret;

    return ret;
}
