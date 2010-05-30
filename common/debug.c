/*
 * =====================================================================================
 *
 *       Filename:  debug.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2010 08:53:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfei Wang (thinXer), me@thinxer.com
 *        Company:  Tsinghua University
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"debug.h"

void die(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
