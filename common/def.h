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
    CWD,
    CDUP,
    LIST,
    NLST,
    SITE,
    STAT,
    HELP,
    NOOP,
    TYPE,
    PASV,
    PORT,
    SYST,
    QUIT,
    MDTM,
    SIZE,
    FTP_CMD_COUNT
};

struct ftp_cmd_str {
    char *str;
    enum FTP_CMD cmd;
};

struct ftp_cmd_str FTP_CMD_STR[FTP_CMD_COUNT];


enum FTP_REPLY {
    REP_OPENDATA = 150,
    REP_OK = 200,
    REP_MDTM = 213,
    REP_SIZE = 213,
    REP_SYST = 215,
    REP_READY = 220,
    REP_QUIT = 221,
    REP_TRANSFERCOMPLETE = 226,
    REP_PASVOK = 227,
    REP_NAMEOK = 331,
    REP_CWD = 250,
    REP_CDUP = 250,
    REP_RNTO = 250,
    REP_DELE = 250,
    REP_PWD = 257,
    REP_MKDIR = 257,
    REP_LOGGEDIN = 230,
    REP_RESTOK = 350,
    REP_RNFR = 350,
    REP_ERRUNKWN = 500,
    REP_ERRPARM = 501,
    REP_ERRIGN = 202,
    REP_ERRNOTIMPL = 502,
    REP_BADSEQ = 503,
    REP_ERRNOTIMPLPARM = 504
};


#define	MSG_OK  "%d OK\r\n"
#define MSG_READY "%d Welcome to thinxer's ftp server\r\n"
#define MSG_NAMEOK "%d User name OK, need password\r\n"
#define	MSG_SYST "%d UNIX\r\n"
#define MSG_LOGGEDIN "%d User logged in\r\n"
#define MSG_ERRNOTIMPL "%d I'm a dumb\r\n"
#define MSG_ERRUNKWN "%d Unkown error\r\n"
#define MSG_PWD "%d \"%s\" is the current directory\r\n"
#define	MSG_CWD "%d CWD command sucessful\r\n"
#define	MSG_CDUP "%d CDUP command sucessful\r\n"
#define MSG_CHANGETYPE "%d Type changed to %c\r\n"
#define MSG_PASV "%d Entering Passive Mode (%d,%d,%d,%d,%d,%d)\r\n"
#define MSG_ENTERASCII "%d Opening ASCII mode data connection for %s\r\n"
#define MSG_ENTERBINARY "%d Opening BINARY mode data connection for %s\r\n"
#define MSG_TRANSFERCOMPLETE "%d Transfer complete\r\n"
#define	MSG_PORT "%d PORT command successful\r\n"
#define	MSG_HELP "%d HELPr\r\n"
#define MSG_REST "%d Restarting at %d. Send STORE or RETRIEVE to initiate transfer\r\n"
#define	MSG_MKDIR "%d \"%s\" created\r\n"
#define	MSG_RNFR "%d Ready for RNTO\r\n"
#define	MSG_RNTO "%d Rename successful\r\n"
#define	MSG_DELE "%d Delete successful\r\n"
#define	MSG_MDTM "%d %s\r\n"
#define	MSG_SIZE "%d %d\r\n"
#define	MSG_QUIT "%d Goodbye\r\n"


enum DATA_TYPE {
    TYPE_ASCII,
    TYPE_EBDIC,
    TYPE_IMAGE
};

enum TRANSFER_TYPE {
    TYPE_PASV,
    TYPE_PORT
};

#endif   /* ----- #ifndef FTPD_DEF_INC  ----- */
