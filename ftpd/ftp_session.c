/*
 * =====================================================================================
 *
 *       Filename:  ftp_session.c
 *
 *    Description:  main ftp process procedures
 *
 *        Version:  0.0
 *        Created:  03/09/2010 10:48:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#include	"ftp_session.h"
#include	"socket_helper.h"
#include	"def.h"
#include	"debug.h"
#include	<string.h>


void process_ftp_session(int sock) {
    char buffer[BUFFER_SIZE];
    int received = 1;

    char welcome_msg[100] = "thinxer's ftp server\n";

    send(sock, welcome_msg, strlen(welcome_msg), 0);

    while (received > 0) {
        received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (received < 0) {
            die("recv");
        } else if (received == 0) {
            break;
        }
        
        if (send(sock, buffer, received, 0) != received) {
            die("send");
        }
    }
 
    close(sock);
}

