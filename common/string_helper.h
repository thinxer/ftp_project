/*
 * =====================================================================================
 *
 *       Filename:  string_helper.h
 *
 *    Description:  string helper
 *
 *        Version:  0.0
 *        Created:  06/04/2010 12:30:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *   Organization:  Tsinghua University
 *
 * =====================================================================================
 */


#ifndef  STRING_HELPER_INC
#define  STRING_HELPER_INC

#include	"def.h"
/* really dangerous functions... */
const char* get_token(const char* str, int token);

const char* get_number(unsigned int* num, const char* buf);
const char* get_address(unsigned int* addr, const char* buf);
const char* get_port(unsigned int* port, const char* buf);
#endif   /* ----- #ifndef STRING_HELPER_INC  ----- */
