#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
 
extern int set_serial(int fd, int nSpeed, int nBits, char nEvent, int nStop);
#endif