#define INCL_DOSMISC
#define INCL_DOSERRORS
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "messages.hpp"

void PrintMessage(char **Table, int count, int msgno)
{
    char MsgBuf[MAX_MSGLEN];
    ULONG len;
    char lang[CCHMAXPATH], file[CCHMAXPATH], *langenv, *tmp;

    /* Change to the language directory if it exists */
    if ((langenv=getenv("LANG"))!=NULL) {
        if ((strcpy(lang, langenv))!=NULL) {
            if ((tmp=strchr(lang, '_'))!=NULL) {
                *tmp=0;
            }
        }
    } else {
        strcpy(lang, ".");
    }
    sprintf(file, "%s\\%s", lang, MSGFILE);

    if (DosGetMessage(Table, count, MsgBuf, MAX_MSGLEN, msgno, file, &len)==NO_ERROR) {
        DosPutMessage((HFILE)2, len, MsgBuf);
    } else {
        fprintf(stderr, "IRM%04i\n", msgno);
    }
    return;
}