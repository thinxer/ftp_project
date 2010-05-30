/*
 * =====================================================================================
 *
 *       Filename:  def.h
 *
 *    Description:  some definations
 *
 *        Version:  0.0
 *        Created:  03/09/2010 08:59:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#ifndef  FTPD_DEF_INC
#define  FTPD_DEF_INC


#define MAX_PENDING_CONNECTIONS 5               /*  */
#define	BUFFER_SIZE             1024			/*  */

#define MAX_CMD_LEN             10              /*  */

#define LISTEN_ADDR                             /*  */
#define	LISTEN_PORT             2121			/*  */

enum FTP_CMD {
    INVALID = -1,
    USER,
    PASS,
    RETR,
    STOR,
    STOU,
    APPE,
    REST,
    RNFR,
    RNTO,
    ABOR,
    DELE,
    RMD,
    MKD,
    PWD,
    LIST,
    NLST,
    SITE,
    STAT,
    HELP,
    NOOP,
    FTP_CMD_COUNT
};

struct ftp_cmd_str {
    char *str;
    enum FTP_CMD cmd;
};

struct ftp_cmd_str FTP_CMD_STR[FTP_CMD_COUNT];


enum FTP_REPLY {
    REP_OK = 200,
    REP_READY = 220,
    REP_NAMEOK = 230,
    REP_PWD = 257,
    REP_LOGGEDIN = 331,
    REP_ERRUNKWN = 500,
    REP_ERRPARM = 501,
    REP_ERRIGN = 202,
    REP_ERRNOTIMPL = 502,
    REP_BADSEQ = 503,
    REP_ERRNOTIMPLPARM = 504
};

#define MSG_READY "Welcome to thinxer's ftp server\r\n"
#define MSG_NAMEOK "User name OK, need password.\r\n"
#define MSG_LOGGEDIN "User logged in.\r\n"
#define MSG_ERRNOTIMPL "I'm a dumb.\r\n"
#define MSG_PWD "\"%s\" is the current directory.\r\n"

#endif   /* ----- #ifndef FTPD_DEF_INC  ----- */
