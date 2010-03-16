/*
 * =====================================================================================
 *
 *       Filename:  socket_helper.c
 *
 *    Description:  a socket helper
 *
 *        Version:  1.0
 *        Created:  03/09/2010 09:04:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#include	"debug.h"
#include	"socket_helper.h"
#include	<string.h>

int create_listen_socket(uint32_t inaddr, uint16_t port, int backlog) {
    int server_sock;
    struct sockaddr_in server;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        die("socket");
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(inaddr);
    server.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        die("bind");
    }

    if (listen(server_sock, backlog) < 0) {
        die("listen");
    }

    return server_sock;
}
