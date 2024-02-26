#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#ifndef __ERRORPRINT_H
#define __ERRORPRINT_H

void DieWithUserMessage(const char *msg);

void DieWithSystemMessage(const char *msg);

void HandleTCPClient(int clientSock);
#endif
