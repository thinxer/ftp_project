/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  the main program
 *
 *        Version:  1.0
 *        Created:  03/08/2010 11:12:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>

#include	"../common/debug.h"
#include	"../common/def.h"
#include	"../common/socket_helper.h"
#include	"ftp_session.h"

void clean_up();

int serversock = -1, clientsock = -1;
int running = 0;
pid_t fpid = 1;

int main(int argc, char* argv[]) {
    struct sockaddr_in client; 
    int port = LISTEN_PORT;

    /* avoid zombie processes */
    signal(SIGCHLD, SIG_IGN);

    /* catch CTRL+C */
    signal(SIGINT, clean_up);
    signal(SIGTERM, clean_up);

    /* parse args */
    if (argc == 2) {
        port = atoi(argv[1]);
    } else {
        printf("usage: %s 2121\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* setup socket */
    serversock = create_listen_socket(INADDR_ANY, port, MAX_PENDING_CONNECTIONS);
    plog("listening on port: %d\n", port);

    /* the main loop for accepting connections */
    running = 1;
    while(running) {
        unsigned int clientlen = sizeof(client);

        clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen);

        if (!running) break;

        if (clientsock < 0) {
            die("accept");
        }
        plog("connected: %s\n", inet_ntoa(client.sin_addr));
        
        fpid = fork();
        if (fpid == -1) {                /* error */
            die("fork");
        } else if (fpid == 0) {          /* child process */
            serversock = -1;
            process_ftp_session(clientsock);
            exit(EXIT_SUCCESS);
        } else if (fpid > 0) {           /* server process */
            clientsock = -1;
            plog("forked: %d\n", fpid);
        }
    }

    /* the only way to terminate normally is to CTRL+C, which will go to clean_up and then return here */

    printf("terminated\n");
    exit(EXIT_SUCCESS);
}

void clean_up() {
    if (fpid > 0)
        printf("\ncleaning up...\n");

    running = 0;
    if (serversock >= 0) {
        printf("closing server socket: %d\n", serversock);
        if (close_socket(&serversock) < 0)
            perror("cs");
    }
    if (clientsock >= 0) {
        printf("closing client socket: %d\n", clientsock);
        if (close_socket(&clientsock)) 
            perror("cc");
    }
}
