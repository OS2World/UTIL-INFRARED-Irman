#define INCL_DOSPROCESS       /* DOS process values (for DosSleep) */
#define INCL_DOSEXCEPTIONS    /* DOS exception values */
#define INCL_DOSSEMAPHORES
#define INCL_ERRORS           /* DOS error values     */
#include <os2.h>
#include <stdio.h>

#include "irmand.hpp"
#include "messages.hpp"
#include "xcpthandler.hpp"

int RegisterExceptionHandler(PEXCEPTIONREGISTRATIONRECORD RegRec)
{
    APIRET rc=NO_ERROR;   // Return code
    ULONG count;

    RegRec->ExceptionHandler=(ERR)MyExceptionHandler;
    rc=DosSetExceptionHandler(RegRec);
    if (rc!=NO_ERROR) {
        return 1;
    }

    rc=DosSetSignalExceptionFocus(SIG_SETFOCUS, &count);
    if (count<1 || rc!=NO_ERROR) {
        return 1;
    }

    PrintMessage(NULL, 0, MSG_QUIT);

    return NO_ERROR;
}
 

int DeRegisterExceptionHandler(PEXCEPTIONREGISTRATIONRECORD RegRec)
{
    APIRET rc=NO_ERROR;
    rc=DosUnsetExceptionHandler(RegRec);
    if (rc!=NO_ERROR) {
        return 1;
    }

    return NO_ERROR;
}

ULONG _System MyExceptionHandler(PEXCEPTIONREPORTRECORD p1, PEXCEPTIONREGISTRATIONRECORD p2, PCONTEXTRECORD p3, PVOID pv)
{
    if (p1->ExceptionNum==XCPT_SIGNAL) {

        switch (p1->ExceptionInfo[0]) {

            // In case of a XCPT_SIGNAL_INTR or XCPT_SIGNAL_BREAK
        case XCPT_SIGNAL_INTR:
        case XCPT_SIGNAL_BREAK:
            {
                // Post a semaphore
                DosPostEventSem(hev);
                /* We continue so we can do some cleanup */
                return XCPT_CONTINUE_EXECUTION;
            } // case
        default:
            {
                return XCPT_CONTINUE_SEARCH;
            } // default
        } // switch
    } // if

    return XCPT_CONTINUE_SEARCH;
}
