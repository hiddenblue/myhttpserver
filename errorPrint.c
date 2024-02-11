#include <stdio.h>
#include <stdlib.h>
#include "errorPrint.h"

void DieWithUserMessage(const char *msg)
{
    fputs(msg, stderr);
    fputs("\n", stderr);
    exit(-1);
}

void DieWithSystemMessage(const char *msg)
{
    fputs(msg, stderr);
    fputs("\n", stderr);
    exit(-1);
}