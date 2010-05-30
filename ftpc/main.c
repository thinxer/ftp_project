/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  a simple ftp client
 *
 *        Version:  0.0
 *        Created:  05/24/2010 11:04:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *   Organization:  Tsinghua University
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	"../common/socket_helper.h"
#include	"../common/debug.h"

int main() {
    int sock;
    char buf[100];
    sock = create_client_socket("127.0.0.1", 2121);
    send(sock, "STOU\r\n", 7, 0);
    recv(sock, buf, 100, 0);
    printf("%s\n", buf);
    printf("%d\n", close_socket(&sock));
    exit(EXIT_SUCCESS);
}
