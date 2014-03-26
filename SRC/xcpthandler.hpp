#ifndef XCPTHANDLER_HPP
#define XCPTHANDLER_HPP

#define INCL_DOSPROCESS       /* DOS process values (for DosSleep) */
#define INCL_DOSEXCEPTIONS    /* DOS exception values */
#define INCL_DOSSEMAPHORES
#define INCL_ERRORS           /* DOS error values     */
#include <os2.h>

#define SEM_IRMAN "\\SEM32\\IRMAN"

ULONG _System MyExceptionHandler(PEXCEPTIONREPORTRECORD, PEXCEPTIONREGISTRATIONRECORD, PCONTEXTRECORD, PVOID);
int RegisterExceptionHandler(PEXCEPTIONREGISTRATIONRECORD);
int DeRegisterExceptionHandler(PEXCEPTIONREGISTRATIONRECORD);

#endif
