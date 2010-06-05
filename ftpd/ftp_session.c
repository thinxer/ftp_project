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

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<unistd.h>
#include	<time.h>

#include	<sys/stat.h>
#include	<sys/types.h>

#include	"ftp_session.h"
#include	"../common/socket_helper.h"
#include	"../common/def.h"
#include	"../common/debug.h"
#include	"../common/string_helper.h"

extern int running;

enum FTP_CMD get_cmd(char* buffer, int buf_len) {
    int i, k;
    for (i=0; i<sizeof(FTP_CMD_STR)/sizeof(FTP_CMD_STR[0]); i++) {
        k = 0;
        while(FTP_CMD_STR[i].str[k] && k < buf_len) {
            if ((FTP_CMD_STR[i].str[k] & 0x1f) != (buffer[k] & 0x1f)) break;
            k++;
        }
        // XXX bug here, prefix damn
        if (FTP_CMD_STR[i].str[k] == '\0')
            return FTP_CMD_STR[i].cmd;
    }
    return INVALID;
}

char* get_cmd_path(const char* buf) {
    static char buffer[BUFFER_SIZE];
    char* p = buffer;
    while (*buf && *buf != ' ') buf++;
    buf++;
    while (*buf && *buf != '\r' && *buf != '\n') {
        *p++ = *buf++;
    }
    *p = 0;
    return buffer;
}

void file_list(int sock, char* dir) {
    char cmd_buffer[BUFFER_SIZE];
    sprintf(cmd_buffer, "ls -l %s", dir);
    FILE* pipe = popen(cmd_buffer, "r");
    send_file(sock, pipe);
    pclose(pipe);
}

struct stat* get_stat(char* path) {
    static struct stat buf;
    if (stat(path, &buf) == 0)
        return &buf;
    else 
        return NULL;
}

void process_ftp_session(int sock) {
    char buffer[BUFFER_SIZE];
    int received = 1;

    struct sockaddr_in serv;
    unsigned int serv_len = sizeof(serv);
    unsigned int addr;

    char cur_path[BUFFER_SIZE] = {0};
    enum DATA_TYPE data_type = TYPE_IMAGE;
    uint32_t pasv_port;
    unsigned int port_address;
    unsigned int  port_port; 
    enum TRANSFER_TYPE transfer_type;
    struct sockaddr_in data_client; 
    unsigned int data_clientlen = sizeof(data_client);
    int data_clientsock = -1;
    int pasv_serversock = -1;

    unsigned int file_rest = 0;
    char rnfr[BUFFER_SIZE];
    
    srand(time(NULL));

    getsockname(sock, (struct sockaddr *) &serv, &serv_len);
    addr = ntohl(serv.sin_addr.s_addr);
    plog("server interface: %s\n", inet_ntoa(serv.sin_addr));


    send_msg(sock, MSG_READY, REP_READY);

    while (received > 0) {
        received = recv(sock, buffer, BUFFER_SIZE, MSG_PEEK);

        if (!running) break;
        if (received < 0) {
            die("recv");
        } else if (received == 0) {
            break;
        }

        buffer[received] = 0;
        printf("%s", buffer);

        int i;
        for (i=0; i<received; i++) {
            //if (buffer[i] == '\r' && buffer[i+1] == '\n') {
            if (buffer[i] == '\n') {
                received = recv(sock, buffer, i+1, 0);
                buffer[received] = 0;
                enum FTP_CMD cmd = get_cmd(buffer, received);
                switch(cmd) {
                    case NOOP:
                        send_msg(sock, MSG_OK, REP_OK);
                        break;
                    case QUIT:
                        send_msg(sock, MSG_QUIT, REP_QUIT);
                        received = -1;
                        break;
                    case HELP:
                        send_msg(sock, MSG_HELP, REP_OK);
                        break;
                    case USER:
                        send_msg(sock, MSG_NAMEOK, REP_NAMEOK);
                        break;
                    case PASS:
                        send_msg(sock, MSG_LOGGEDIN, REP_LOGGEDIN);
                        break;
                    case PWD:
                        getcwd(cur_path, sizeof(cur_path));
                        send_msg(sock, MSG_PWD, REP_PWD, cur_path);
                        break;

                    case SYST:
                        send_msg(sock, MSG_SYST, REP_SYST);
                        break;
                    case TYPE:
                        switch(buffer[5]) {
                            case 'A':
                                data_type = TYPE_ASCII;
                                send_msg(sock, MSG_CHANGETYPE, REP_OK, buffer[5]);
                                break;
                            case 'I':
                                data_type = TYPE_IMAGE;
                                send_msg(sock, MSG_CHANGETYPE, REP_OK, buffer[5]);
                                break;
                            default:
                                send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                                break;
                        }
                        break;

                    case PASV:
                        do {
                            pasv_port = rand();
                        } while (pasv_port < 1024 || pasv_port > 65535);

                        transfer_type = TYPE_PASV;

                        pasv_serversock = create_listen_socket(INADDR_ANY, pasv_port, 1);
                        send_msg(sock, MSG_PASV, REP_PASVOK, addr >> 24, (addr >> 16) & 0xff, (addr >> 8) & 0xff, addr & 0xff, pasv_port >> 8, pasv_port & 0xff);
                        break;

                    case PORT:
                        transfer_type = TYPE_PORT;
                        {
                            const char* tmp = buffer;
                            tmp = get_address(&port_address, tmp);
                            tmp = get_port(&port_port, tmp);
                            plog("PORT: %d %d\n", port_address, port_port);
                            send_msg(sock, MSG_PORT, REP_OK);
                        }
                        break;
                        
                    case LIST:
                        if (transfer_type == TYPE_PASV && pasv_port > 0) {
                            if (pasv_serversock < 0)
                                die("pasv sock");
                            send_msg(sock, MSG_ENTERASCII, REP_OPENDATA, "LIST");
                            data_clientsock = accept(pasv_serversock, (struct sockaddr *) &data_client, &data_clientlen);
                            if (data_clientsock < 0)
                                die("pasv sock accept");
                        } else if (transfer_type == TYPE_PORT && port_address>0 && port_port>0) {
                            send_msg(sock, MSG_ENTERASCII, REP_OPENDATA, "LIST");
                            data_clientsock = create_client_socket(port_address, port_port);
                            if (data_clientsock < 0)
                                die("port data sock");
                        }

                        if (data_clientsock > 0) {
                            getcwd(cur_path, sizeof(cur_path));
                            file_list(data_clientsock, cur_path);
                            send_msg(sock, MSG_TRANSFERCOMPLETE, REP_TRANSFERCOMPLETE);
                        } else {
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        }

                        close_socket(&data_clientsock);
                        close_socket(&pasv_serversock);
                        break;
                    case REST:
                        get_number(&file_rest, buffer);
                        send_msg(sock, MSG_REST, REP_RESTOK, file_rest);
                        break;
                    case RETR:
                    case STOR:
                        if (transfer_type == TYPE_PASV && pasv_port > 0) {
                            if (pasv_serversock < 0)
                                die("pasv sock");
                            if (data_type == TYPE_ASCII)
                                send_msg(sock, MSG_ENTERASCII, REP_OPENDATA, "data");
                            else 
                                send_msg(sock, MSG_ENTERBINARY, REP_OPENDATA, "data");
                            data_clientsock = accept(pasv_serversock, (struct sockaddr *) &data_client, &data_clientlen);
                            if (data_clientsock < 0)
                                die("pasv sock accept");
                        } else if (transfer_type == TYPE_PORT && port_address>0 && port_port>0) {
                            if (data_type == TYPE_ASCII)
                                send_msg(sock, MSG_ENTERASCII, REP_OPENDATA, "data");
                            else 
                                send_msg(sock, MSG_ENTERBINARY, REP_OPENDATA, "data");
                            data_clientsock = create_client_socket(port_address, port_port);
                            if (data_clientsock < 0)
                                die("port data sock");
                        }

                        if (cmd == RETR) {
                            if (send_local_file(data_clientsock, get_cmd_path(buffer), file_rest) == 0)  {
                                send_msg(sock, MSG_TRANSFERCOMPLETE, REP_TRANSFERCOMPLETE);
                                file_rest = 0;
                            } else
                                send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        } else if (cmd == STOR) {
                            if (receive_local_file(data_clientsock, get_cmd_path(buffer), file_rest) == 0) {
                                send_msg(sock, MSG_TRANSFERCOMPLETE, REP_TRANSFERCOMPLETE);
                                file_rest = 0;
                            } else
                                send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        }

                        close_socket(&data_clientsock);
                        close_socket(&pasv_serversock);
                        break;

                    case CDUP:
                        if (chdir("..") == 0)
                            send_msg(sock, MSG_CDUP, REP_CDUP);
                        else
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        break;

                    case CWD:
                        if (chdir(get_cmd_path(buffer)) == 0)
                            send_msg(sock, MSG_CWD, REP_CWD);
                        else
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        break;

                    case MDTM:
                        {
                            struct stat* info = get_stat(get_cmd_path(buffer));
                            if (info) {
                                struct tm mtime;
                                char time_buf[BUFFER_SIZE];
                                gmtime_r(&(info->st_mtime), &mtime);
                                strftime(time_buf, sizeof(time_buf), "%Y%m%d%H%M%S", &mtime);
                                send_msg(sock, MSG_MDTM, REP_MDTM, time_buf);
                            } else
                                send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        }
                        break;

                    case SIZE:
                        {
                            struct stat* info = get_stat(get_cmd_path(buffer));
                            if (info) 
                                send_msg(sock, MSG_SIZE, REP_SIZE, info->st_size);
                            else
                                send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        }
                        break;

                    case DELE:
                        if (remove(get_cmd_path(buffer))==0) 
                            send_msg(sock, MSG_DELE, REP_DELE);
                        else
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        break;

                    case RMD:
                        if (rmdir(get_cmd_path(buffer))==0) 
                            send_msg(sock, MSG_DELE, REP_DELE);
                        else
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        break;

                    case MKD:
                        if (mkdir(get_cmd_path(buffer), 0777) == 0)
                            send_msg(sock, MSG_MKDIR, REP_MKDIR, get_cmd_path(buffer));
                        else
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        break;

                    case RNFR:
                        strcpy(rnfr, get_cmd_path(buffer));
                        send_msg(sock, MSG_RNFR, REP_RNFR);
                        break;

                    case RNTO:
                        if (rename(rnfr, get_cmd_path(buffer)) == 0)
                            send_msg(sock, MSG_RNTO, REP_RNTO);
                        else
                            send_msg(sock, MSG_ERRUNKWN, REP_ERRUNKWN);
                        break;

                    default:
                        send_msg(sock, MSG_ERRNOTIMPL, REP_ERRNOTIMPL);
                        break;
                }
                break;
            }
        }
    }

    // clean up
    close_socket(&pasv_serversock);
    close_socket(&data_clientsock);
    close_socket(&sock);
}

