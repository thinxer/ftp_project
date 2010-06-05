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
#include	<string.h>

#include	"../common/socket_helper.h"
#include	"../common/debug.h"
#include	"../common/def.h"
#include	"../common/string_helper.h"
#include	"main.h"

struct ftp_cmd_str CLIENT_CMD_STR[C_COUNT] = {
    {"LS", C_LS},
    {"PWD", C_PWD},
    {"CD", C_CD},
    {"CDUP", C_CDUP},
    {"RENAME", C_RENAME},
    {"PUT", C_PUT},
    {"GET", C_GET},
    {"USER", C_USER},
    {"PASS", C_PASS},
    {"TYPE", C_TYPE},
    {"BYE", C_BYE},
    {"MKD", C_MKD},
    {"DELE", C_DELE},
    {"RNFR", C_RNFR},
    {"RNTO", C_RNTO},
    {"RMD", C_RMD},

    {"LCD", C_LCD},
    {"LLS", C_LLS},
    {"LPWD", C_LPWD},

    {"HELP", C_HELP}
};

enum CLIENT_CMD get_cmd(char* buffer, int buf_len) {
    int i, k;
    for (i=0; i<sizeof(CLIENT_CMD_STR)/sizeof(CLIENT_CMD_STR[0]); i++) {
        k = 0;
        while(CLIENT_CMD_STR[i].str[k] && k < buf_len) {
            if ((CLIENT_CMD_STR[i].str[k] & 0x1f) != (buffer[k] & 0x1f)) break;
            k++;
        }
        if (CLIENT_CMD_STR[i].str[k] == '\0' && (buffer[k]==' ' || buffer[k]==0))
            return CLIENT_CMD_STR[i].cmd;
    }
    return C_INVALID;
}

void file_list(char* dir) {
    char cmd_buffer[BUFFER_SIZE];
    sprintf(cmd_buffer, "ls -l %s", dir);
    FILE* pipe = popen(cmd_buffer, "r");
    
    char buf[BUFFER_SIZE];
    int n;
    while ((n=fread(buf, 1, BUFFER_SIZE, pipe)) > 0) {
        fwrite(buf, 1, n, stdout);
    }

    pclose(pipe);
    return 0;
}

int main(int argc, char* argv[]) {

    int sock;
    char buf[BUFFER_SIZE];
    int received = 1;
    int port = 21;
    char line[BUFFER_SIZE];
    char cur_path[BUFFER_SIZE];
    char filename[BUFFER_SIZE];

    int running = 1;

    enum CLIENT_STATE state;

    struct sockaddr_in serv;
    unsigned int serv_len = sizeof(serv);
    unsigned int addr;

    enum DATA_TYPE data_type = TYPE_IMAGE;
    unsigned int port_port;
    unsigned int pasv_address;
    unsigned int  pasv_port; 
    enum TRANSFER_TYPE transfer_type;
    struct sockaddr_in data_client; 
    unsigned int data_clientlen = sizeof(data_client);
    int pasv_serversock = -1;

    int data_sock = -1;

    if (argc != 2 && argc != 3) {
        printf("usage: %s 127.0.0.1 [2121]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 3)
        port = atoi(argv[2]);

    sock = create_client_socket_char(argv[1], port);
    if (sock < 0)
        die("create_client_sock");

    while (received > 0 && running) {
        received = recv(sock, buf, BUFFER_SIZE, MSG_PEEK);
        if (received == 0)
            break;
        else if (received < 0)
            die("recv");

        buf[received] = 0;
        printf("%s", buf);
        fflush(stdout);

        int i;
        for (i=0; i<received; i++) {
            if (buf[i] == '\n') {
                received = recv(sock, buf, i+1, 0);

                int reply_code = atoi(get_token(buf, 0));

                // printf("state: %d\n", state);

                if (reply_code < 500 && state != S_NONE) {
                    switch(state) {
                        case S_PASVLIST:
                        case S_PASVGET:
                        case S_PASVPUT:
                            if (reply_code == 227)
                            {
                                char* p=buf+3;
                                p = get_address(&pasv_address, p);
                                p = get_port(&pasv_port, p);
                                switch(state) {
                                    case S_PASVLIST:
                                        send_msg(sock, "LIST\r\n");
                                        state = S_PASVLIST_2;
                                        break;
                                    case S_PASVGET:
                                        send_msg(sock, "RETR %s\r\n", filename);
                                        state = S_PASVGET_2;
                                        break;
                                    case S_PASVPUT:
                                        send_msg(sock, "STOR %s\r\n", filename);
                                        state = S_PASVPUT_2;
                                        break;
                                }
                                data_sock = create_client_socket(pasv_address, pasv_port);
                            } else {
                                state = S_NONE;
                            }
                            break;
                        case S_PASVLIST_2:
                        case S_PASVGET_2:
                        case S_PASVPUT_2:
                            {
                                if (data_sock < 0)
                                    printf("error creating data connection\n");
                                else {
                                    if (state == S_PASVLIST_2) {
                                        receive_file(data_sock, stdout);
                                    } else if (state == S_PASVGET_2) {
                                        receive_local_file(data_sock, filename, 0);
                                    } else if (state == S_PASVPUT_2) {
                                        FILE* f = fopen(filename, "rb");
                                        if (f) {
                                            send_file(data_sock, f);
                                            fclose(f);
                                        } else
                                            printf("error open file\n");
                                    }
                                }
                                close_socket(&data_sock);
                                state = S_NONE;
                            }
                            break;
                        default:
                            state = S_NONE;
                            break;
                    }
                    if (reply_code < 500)
                        break;
                }

                if (reply_code >= 500) state = S_NONE;

                int valid = 0;
                while(!valid) {
                    valid = 1;
                    printf("ftpc>");
                    if (!fgets(line, BUFFER_SIZE, stdin)) {
                        running = 0;
                        break;
                    }

                    // strip \r\n
                    int len = strlen(line);
                    len --;
                    while (line[len] == '\n' || line[len] == '\r') len --;
                    len ++;
                    line[len] = 0;

                    enum CLIENT_CMD cmd = get_cmd(line, len);
                    // printf("%s:%d\n", line, cmd);
                    switch(cmd) {
                        case C_USER:
                        case C_PASS:
                        case C_TYPE:
                        case C_MKD:
                        case C_DELE:
                        case C_RNFR:
                        case C_RNTO:
                        case C_RMD:
                            send_msg(sock, "%s\r\n", line);
                            break;
                        case C_LS:
                            send_msg(sock, "PASV\r\n");
                            state = S_PASVLIST;
                            break;
                        case C_CD:
                            send_msg(sock, "CWD %s\r\n", get_token(line, 1));
                            break;
                        case C_PWD:
                            send_msg(sock, "PWD\r\n");
                            break;
                        case C_CDUP:
                            send_msg(sock, "CDUP\r\n");
                            break;
                        case C_HELP:
                            printf(HELP_STRING);
                            for (i=0; i<sizeof(CLIENT_CMD_STR)/sizeof(CLIENT_CMD_STR[0]); i++) {
                                printf("%s\n", CLIENT_CMD_STR[i].str);
                            }
                            valid = 0;
                            break;
                        case C_BYE:
                            send_msg(sock, "QUIT\r\n");
                            running = 0;
                            break;
                        case C_LCD:
                            chdir(get_token(line, 1));
                            valid = 0;
                            break;
                        case C_LLS:
                            getcwd(cur_path, sizeof(cur_path));
                            file_list(cur_path);
                            valid = 0;
                            break;
                        case C_LPWD:
                            getcwd(cur_path, sizeof(cur_path));
                            printf("%s\n", cur_path);
                            valid = 0;
                            break;
                        case C_GET:
                            send_msg(sock, "PASV\r\n");
                            strcpy(filename, get_token(line, 1));
                            state = S_PASVGET;
                            break;
                        case C_PUT:
                            send_msg(sock, "PASV\r\n");
                            strcpy(filename, get_token(line, 1));
                            state = S_PASVPUT;
                            break;
                        default:
                            printf("unknown command\n");
                            valid = 0;
                            break;
                    }
                }
                break;
            }
        }
    }

    printf("finished: %d\n", close_socket(&sock));
    exit(EXIT_SUCCESS);
}
