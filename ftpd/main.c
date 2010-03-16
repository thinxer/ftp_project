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


#include	"debug.h"
#include	"def.h"
#include	"socket_helper.h"
#include	"ftp_session.h"

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<signal.h>


int main(int argc, char* argv[]) {
    int serversock, clientsock;
    struct sockaddr_in client; 
    pid_t fpid;                         /* the forked server pid */

    /* avoid zombie processes */
    signal(SIGCHLD, SIG_IGN);

    /* setup socket */
    serversock = create_listen_socket(INADDR_ANY, LISTEN_PORT, MAX_PENDING_CONNECTIONS);

    /* the main loop for accepting connections */
    for (;;) {
        unsigned int clientlen = sizeof(client);

        clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen);
        if (clientsock < 0) {
            die("accept");
        }
        printf("connected: %s\n", inet_ntoa(client.sin_addr));
        
        fpid = fork();
        if (fpid == -1) {                /* error */
            die("fork");
        } else if (fpid == 0) {          /* child process */
            close(serversock);
            process_ftp_session(clientsock);
            exit(EXIT_SUCCESS);
        } else if (fpid > 0) {           /* server process */
            close(clientsock);
            printf("forked\n");
        }
    }
    exit(EXIT_SUCCESS);
}
