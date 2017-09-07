#ifndef SYSUTIL_H
#define SYSUTIL_H

//add the system call header here

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <errno.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

int passiveSocketBindListen();
int connect(uint32_t address, uint16_t port);
#endif
