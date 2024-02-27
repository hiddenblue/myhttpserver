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
#include "printDNSAddr.h"

#ifndef __TCPUTILITY_H
#define __TCPUTILITY_H

int setupTCPServerconn(const char *servername, const char *port);

#endif