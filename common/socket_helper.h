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
#include	<stdio.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<unistd.h>

/* create a socket, bind and listen to it */
int create_listen_socket(unsigned int inaddr, unsigned short port, int backlog); 
/* create a client socket, connect to the server */
int create_client_socket(unsigned int server_address, unsigned short port);
int create_client_socket_char(char* server_address, unsigned short port);
int close_socket(int* sock);

int send_msg(int sock, const char* fmt, ...);
int send_file(int sock, FILE* file);
int send_local_file(int sock, char* file, unsigned int rest);
int receive_file(int sock, FILE* file);
int receive_local_file(int sock, char* path, unsigned int rest);
#endif   /* ----- #ifndef SOCKET_HELPER_INC  ----- */
