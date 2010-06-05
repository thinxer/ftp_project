/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  for debug output and die function
 *
 *        Version:  0.0
 *        Created:  03/09/2010 08:51:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#ifndef  FTPD_DEBUG_INC
#define  FTPD_DEBUG_INC

void die(char *msg);
void plog(char* fmt, ...);

#endif   /* ----- #ifndef FTPD_DEBUG_INC  ----- */

