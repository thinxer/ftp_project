/*
 * =====================================================================================
 *
 *       Filename:  socket_helper.h
 *
 *    Description:  a socket helper
 *
 *        Version:  1.0
 *        Created:  03/09/2010 08:39:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#ifndef  SOCKET_HELPER_INC
#define  SOCKET_HELPER_INC

#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<unistd.h>

int create_listen_socket(uint32_t inaddr, uint16_t port, int backlog); /* create a socket, bind and listent to it */
int create_client_socket(char* server_address, unsigned short port);
int close_socket(int* sock);

#endif   /* ----- #ifndef SOCKET_HELPER_INC  ----- */
