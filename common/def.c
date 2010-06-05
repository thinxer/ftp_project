/*
 * =====================================================================================
 *
 *       Filename:  def.c
 *
 *    Description:  some definations
 *
 *        Version:  0.0
 *        Created:  05/24/2010 10:15:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *   Organization:  Tsinghua University
 *
 * =====================================================================================
 */

#include	"def.h"

struct ftp_cmd_str FTP_CMD_STR[FTP_CMD_COUNT] = {
    {"USER", USER},
    {"PASS", PASS},
    {"RETR", RETR},
    {"STOR", STOR},
    {"STOU", STOU},
    {"APPE", APPE},
    {"REST", REST},
    {"RNFR", RNFR},
    {"RNTO", RNTO},
    {"ABOR", ABOR},
    {"DELE", DELE},
    {"RMD" , RMD },
    {"MKD" , MKD },
    {"PWD" , PWD },
    {"CWD" , CWD },
    {"CDUP", CDUP},
    {"LIST", LIST},
    {"NLST", NLST},
    {"SITE", SITE},
    {"STAT", STAT},
    {"HELP", HELP},
    {"TYPE", TYPE},
    {"PASV", PASV},
    {"PORT", PORT},
    {"SYST", SYST},
    {"NOOP", NOOP},
    {"QUIT", QUIT},
    {"MDTM", MDTM},
    {"SIZE", SIZE}
};

