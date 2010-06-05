/*
 * =====================================================================================
 *
 *       Filename:  string_helper.c
 *
 *    Description:  string helper
 *
 *        Version:  0.0
 *        Created:  06/04/2010 12:35:43 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *   Organization:  Tsinghua University
 *
 * =====================================================================================
 */

#include	<ctype.h>
#include	"string_helper.h"

const char* get_token(const char* str, int token) {
    static char buf[BUFFER_SIZE];
    char* p = buf;
    while(token > 0) {
        while(*str && *str != ' ' && *str != '\r' && *str != '\n') str++;
        token --;
        str++;
        if (*str == '\n') str++;
    }
    while(*str != ' ' && *str != '\r' && *str != '\n') {
        *p++ = *str++;
    }
    *p = 0;
    return buf;
}

const char* get_number(unsigned int* num, const char* buf) {
    *num = 0;
    while (!isdigit(*buf)) buf++;
    while (isdigit(*buf)) {
        *num = *num * 10 + (*buf - '0');
        buf++;
    }
    return buf;
}
const char* get_address(unsigned int* addr, const char* buf) {
    unsigned int tmp;
    int i;
    *addr = 0;
    for (i=0; i<4; i++) {
        buf = get_number(&tmp, buf);
        *addr = (*addr<<8) + tmp;
    }
    return buf;
}
const char* get_port(unsigned int* port, const char* buf) {
    unsigned int tmp;
    int i;
    *port = 0;
    for (i=0; i<2; i++) {
        buf = get_number(&tmp, buf);
        *port = (*port<<8) + tmp;
    }
    return buf;
}
