/*
 * =====================================================================================
 *
 *       Filename:  main.h
 *
 *    Description:  some definitions
 *
 *        Version:  0.0
 *        Created:  06/04/2010 04:23:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *   Organization:  Tsinghua University
 *
 * =====================================================================================
 */

#ifndef  MAIN_INC
#define  MAIN_INC

#include	"../common/def.h"

enum CLIENT_CMD {
    C_INVALID = -1,
    C_LS = 0,
    C_PWD,
    C_CD,
    C_CDUP,
    C_RENAME,
    C_PUT,
    C_GET,
    C_USER,
    C_PASS,
    C_TYPE,
    C_BYE,
    C_MKD,
    C_DELE,
    C_RNFR,
    C_RNTO,
    C_RMD,

    C_LCD,
    C_LLS,
    C_LPWD,

    C_HELP,

    C_COUNT
};

enum CLIENT_STATE {
    S_NONE,
    S_PASVLIST,
    S_PASVLIST_2,

    S_PASVGET,
    S_PASVGET_2,

    S_PASVPUT,
    S_PASVPUT_2
};

#define HELP_STRING "help string... here... \n"

struct ftp_cmd_str CLIENT_CMD_STR[C_COUNT];

#endif   /* ----- #ifndef MAIN_INC  ----- */
