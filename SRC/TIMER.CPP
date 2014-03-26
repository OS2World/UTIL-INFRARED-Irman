// OS/2 defines
#define INCL_DOSERRORS
#define INCL_DOSDATETIME
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include "messages.hpp"
#include "timer.hpp"

Timer::Timer(ULONG ulMyInterval)
{
   rc=DosCreateEventSem(NULL, &hevSemaphore, DC_SEM_SHARED, FALSE);
   if (rc!=NO_ERROR)
   {
      PrintMessage(NULL, 0, MSG_SEMERROR);
      return;
   }

   SetInterval(ulMyInterval);
   Start();
}

Timer::~Timer()
{
   Stop();
   rc=DosCloseEventSem(hevSemaphore);
}

BOOL Timer::Reset()
{
   if (Stop()) {
      return Start();
   } /* endif */
   return FALSE;
}

BOOL Timer::Stop()
{
   rc=DosStopTimer(htTimer);
   // We ignore handle errors because the handle is invalid if the timer has expired.
   if (rc!=NO_ERROR && rc !=ERROR_TS_HANDLE)
   {
       PrintMessage(NULL, 0, MSG_ERRORTIMER);
       return FALSE;
   }
   return TRUE;
}

BOOL Timer::Start()
{
   ULONG ulcount;
   rc=DosResetEventSem(hevSemaphore, &ulcount);
   rc=DosAsyncTimer(ulInterval, (HSEM) hevSemaphore, &htTimer);
   if (rc!=NO_ERROR)
      {
       PrintMessage(NULL, 0, MSG_ERRORTIMER);
      return FALSE;
      }
   return TRUE;
}


ULONG Timer::IsPosted()
{
   ULONG ulCount=0;

   rc=DosQueryEventSem(hevSemaphore, &ulCount);
   if (rc!=NO_ERROR)
      {
       PrintMessage(NULL, 0, MSG_ERRORTIMER);
         return 0;
      }
   return ulCount;
}

BOOL Timer::Wait(ULONG ulTimeOut)
{
   rc=DosWaitEventSem(hevSemaphore, ulTimeOut);
   if (rc!=NO_ERROR)
      {
       PrintMessage(NULL, 0, MSG_ERRORTIMER);
      return FALSE;
      }
   return TRUE;
}

ULONG Timer::SetInterval(ULONG ulMyInterval)
{
   ulInterval=ulMyInterval;
   return ulInterval;
}

APIRET Timer::LastError()
{
    return rc;
}