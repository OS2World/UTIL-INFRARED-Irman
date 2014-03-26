#define INCL_REXXSAA
#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <os2.h>
#include <rexxsaa.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rexxfunc.hpp"
#include "messages.hpp"

int RegisterFunctions()
{
    return RexxRegisterFunctionExe(RX_STARTPROGRAM, (PFN)IrStartProgram);
}

int DeRegisterFunctions()
{
    return RexxDeregisterFunction(RX_STARTPROGRAM);
}

void _Optlink EventHandler(void *XString)
{
    SHORT RxRC;
    RXSTRING RxArgs[RX_ARGS], RxReturn;
    char Buffer[CCHMAXPATH];

    MAKERXSTRING(RxReturn, Buffer, CCHMAXPATH);
    MAKERXSTRING(RxArgs[0], (char *)XString, strlen((char *)XString));
    RexxStart(1, RxArgs, RX_SCRIPT, NULL, NULL, RXFUNCTION, NULL, &RxRC, &RxReturn);
    /* If the REXX interpreter had to extend the buffer we have to free it */
    if (RXSTRPTR(RxReturn)!=Buffer) {
        DosFreeMem(RXSTRPTR(RxReturn));
    }
    /* Only we can free the buffer for the parameters passed */
    free(XString);
    return;
}


ULONG APIENTRY IrStartProgram(PSZ Name,
                            LONG Argc,
                            RXSTRING Argv[],
                            PSZ Queuename,
                            PRXSTRING Retstr)
{
    APIRET rc=NO_ERROR;
    LONG i;
    PID pid;
    ULONG Session;
    STARTDATA sd={0};

    sprintf(Retstr->strptr, "ERROR:");
    Retstr->strlength=strlen(Retstr->strptr);

    /* Do we have sufficient parameters? */
    if ((Argc<RX_MIN_PARAMS) || (Argc>RX_MAX_PARAMS)) return 40;

    /* Setup the startdata */
    sd.Length=sizeof(STARTDATA);
    sd.Related=SSF_RELATED_INDEPENDENT;

    /* Do we have special options? */
    /* Default options */
    sd.FgBg=SSF_FGBG_FORE;
    sd.InheritOpt=SSF_INHERTOPT_SHELL;
    sd.SessionType=SSF_TYPE_DEFAULT;

    /* Options are case sensetive */
    if (Argc>RX_OPTIONS) {
        if (RXZEROLENSTRING(Argv[RX_OPTIONS]) || RXNULLSTRING(Argv[RX_OPTIONS])) {
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'B'))
                sd.FgBg=SSF_FGBG_BACK;
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'I'))
                sd.InheritOpt=SSF_INHERTOPT_PARENT;
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'P'))
                sd.SessionType=SSF_TYPE_PM;
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'V'))
                sd.SessionType=SSF_TYPE_VDM;
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'v'))
                sd.SessionType=SSF_TYPE_WINDOWEDVDM;
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'O'))
                sd.SessionType=SSF_TYPE_FULLSCREEN;
            if (strchr(RXSTRPTR(Argv[RX_OPTIONS]), 'o'))
                sd.SessionType=SSF_TYPE_WINDOWABLEVIO;
        }
    }

    if (RXZEROLENSTRING(Argv[RX_PROGRAMNAME]) || RXNULLSTRING(Argv[RX_PROGRAMNAME])) {
        return 40;
    } else {
        sd.PgmName=RXSTRPTR(Argv[RX_PROGRAMNAME]);
    }
    if (Argc>RX_PARAMETERS) {
        sd.PgmInputs=RXSTRPTR(Argv[RX_PARAMETERS]);
    }

    if (Argc>RX_ENVIRONMENT) {
        if (RXZEROLENSTRING(Argv[RX_ENVIRONMENT]) || RXNULLSTRING(Argv[RX_ENVIRONMENT])) {
            sd.Environment=NULL;
        } else {
            sd.Environment=RXSTRPTR(Argv[RX_ENVIRONMENT]);
        }
    }


    rc=DosStartSession(&sd, &Session, &pid);
    if (rc!=NO_ERROR) {
        PrintMessage(NULL, 0, MSG_STARTERROR);
    }

    sprintf(Retstr->strptr, "%d", rc);
    Retstr->strlength=strlen(Retstr->strptr);

    return 0;
}

