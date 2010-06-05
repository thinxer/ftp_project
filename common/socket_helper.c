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

#include	<string.h>
#include	<stdarg.h>

#include	"debug.h"
#include	"socket_helper.h"
#include	"def.h"

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

int create_client_socket_char(char* server_address, unsigned short port) {
    unsigned int s = ntohl(inet_addr(server_address));
    return create_client_socket(s, port);
}

int create_client_socket(unsigned int server_address, unsigned short port) {
    struct sockaddr_in server;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        die("socket");
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(server_address);
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        die("connect");
    }

    return sock;
}

int close_socket(int* sock) {
    // shutdown(*sock, SHUT_RDWR);
    if (*sock < 0) return 0;
    int ret = close(*sock);
    *sock = -1;
    return ret;
}

int send_msg(int sock, const char* fmt, ...) {
    int ret = 0;
    char buffer[BUFFER_SIZE];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    ret = send(sock, buffer, strlen(buffer), 0);

    return ret;
}

int send_file(int sock, FILE* file) {
    char buf[BUFFER_SIZE];
    int n;
    while ((n=fread(buf, 1, BUFFER_SIZE, file)) > 0) {
        send(sock, buf, n, 0);
    }
    return 0;
}

int send_local_file(int sock, char* file, unsigned int rest) {
    FILE* f = fopen(file, "rb");
    if (f) {
        fseek(f, rest, SEEK_SET);
        send_file(sock, f);
    }
    return fclose(f);
}

int receive_file(int sock, FILE* file) {
    char buffer[BUFFER_SIZE];
    int received = 1;
    do {
        received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (received > 0) {
            fwrite(buffer, 1, received, file);
        }
    } while (received > 0);
    return received;
}

int receive_local_file(int sock, char* path, unsigned int rest) {
    FILE* f = fopen(path, "wb");
    int ret = 0;
    if (f) {
        fseek(f, rest, SEEK_SET);
        ret = receive_file(sock, f);
        fclose(f);
    } else {
        ret = -1;
    }
    return ret;
}
