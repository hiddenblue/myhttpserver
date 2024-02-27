#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "errorPrint.h"

#ifndef __PRINTDNSADDR_H
#define __PRINTDNSADDR_H

void PrintSockaddr(struct sockaddr *sockAddr, FILE *stream);

#endif