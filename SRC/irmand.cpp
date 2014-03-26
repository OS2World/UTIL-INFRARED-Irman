// OS/2 defines
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_DOSDEVIOCTL
#define INCL_DOSDEVICES
#define INCL_DOSPROCESS
#define INCL_DOSSESMGR
#define INCL_DOSDATETIME
#define INCL_DOSSEMAPHORES
#define INCL_DOSEXCEPTIONS
#define INCL_KBD
#include <os2.h>
#include <rexxsaa.h>

// C++ includes
#include <iostream.h>
// C includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <signal.h>

#include "irmand.hpp"
#include "timer.hpp"
#include "irdata.hpp"
#include "irman.hpp"
#include "rexxfunc.hpp"
#include "xcpthandler.hpp"
#include "messages.hpp"

#define VERSION "1.0.20020602"

HEV hev;

void PrintCopyright(void)
{
    char *table={VERSION};
    PrintMessage(&table, 1, MSG_PRODUCT);
    PrintMessage(NULL, 0, MSG_COPYRIGHT);
    PrintMessage(NULL, 0, MSG_LICENSE);
    return;
}

void PrintHelp(void)
{
    PrintMessage(NULL, 0, MSG_USAGE);
    PrintMessage(NULL, 0, MSG_PARAMETERS);
    return;
}

int main(int argc, char *argv[])
{
    EXCEPTIONREGISTRATIONRECORD RegRec={0};
    Irman *anIrman;
    char emptystring[6]={0, 0, 0, 0, 0, 0};
    IrData myIrData, emptyIrData(emptystring);
    char port[6];
    int portno, rc;
    FILE *logf;
    ULONG count=0;

    PrintCopyright();

    /* Process arguments */
    if (argc>2) {
        PrintHelp();
        return -1;
    } else {
        if (argc==2) {
            sscanf(argv[1], "%i", &portno);
            if (portno>0 && portno<=16) {
                sprintf(port, "COM%i", portno);
            } else {
                PrintHelp();
                return -1;
            } // endif portno>0 && portno<=16
        } else {
            *port=0;
        } // argc==2
    } // argc>2

    if ((rc=DosCreateEventSem(SEM_IRMAN, &hev, 0, FALSE))!=NO_ERROR) {
        if (rc==ERROR_DUPLICATE_NAME) {
            PrintMessage(NULL, 0, MSG_INSTANCE);
        } else {
            PrintMessage(NULL, 0, MSG_SEMERROR);
        }
        return -1;
    }

    if ((rc=RegisterFunctions())!=0) {
        PrintMessage(NULL, 0, MSG_REXXERROR);
        return -1;
    }

    if ((rc=RegisterExceptionHandler(&RegRec))!=0) {
        PrintMessage(NULL, 0, MSG_XCPTERROR);
        DeRegisterFunctions();
        return -1;
    }

    /* Create an Irman object */
    anIrman=new Irman(port);
    rc=0;
    /* Start the real work */
    try {

        if (anIrman->LastError()!=NO_ERROR) throw "Failed";

        while (count==0) {
            myIrData=anIrman->ReadData();
            if (myIrData!=emptyIrData) {
                /* Start a event handler */
                _beginthread(EventHandler, NULL, 8192, (void *)myIrData.GetXString());
            }
            DosQueryEventSem(hev, &count);
        } // endwhile

        /* Delete the Irman object */
        delete(anIrman);
        PrintMessage(NULL, 0, MSG_NORMALEND);

    } // endtry
    catch (...) {
        PrintMessage(NULL, 0, MSG_ERROREND);
        rc=-1;
    } // endcatch

    DeRegisterExceptionHandler(&RegRec);
    DeRegisterFunctions();
    DosCloseEventSem(hev);
return rc;
}
