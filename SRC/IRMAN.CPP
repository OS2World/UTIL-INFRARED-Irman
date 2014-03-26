// Implementatie van de Irman Class

// OS/2 defines
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_DOSDEVIOCTL
#define INCL_DOSDEVICES
#define INCL_DOSPROCESS
#define INCL_DOSDATETIME
#define INCL_DOSSEMAPHORES

#include <os2.h>

// C++ includes
#include <iostream.h>
// C includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "messages.hpp"
#include "timer.hpp"
#include "irdata.hpp"
#include "irman.hpp"

Irman::Irman(char *port)
{
   ULONG Action=0;
   char *ComPort;
   int i;

   // Fail if the timer failed
   if ((rc=tRepeatDelay.LastError())!=NO_ERROR)
   {
       return;
   }

   if ((ComPort=(char *)malloc(6))==NULL) {
       rc=(!NO_ERROR);
       return;
   }

   // Setup timer
   tRepeatDelay.SetInterval(REPEATDELAY);
   tRepeatDelay.Reset();

   // Setup Irman
   i=1;
   while (i<=16) {
       if (port[0]==0) {
           sprintf(ComPort, "COM%i", i);
           i++;
       } else {
           sprintf(ComPort, port);
           i=17;
       }
       PrintMessage((char **)&ComPort, 1, MSG_COMPORT);

       rc=DosOpen(ComPort, (PHFILE) &hfComPort, &Action, 0,
                  FILE_NORMAL, FILE_OPEN,
                  OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE, (PEAOP2) NULL);

       if (rc==NO_ERROR) {

           if (ComSetup()==TRUE)
           {
               if (Reset()==TRUE) {
                   PrintMessage((char **)&ComPort, 1, MSG_IRMANFOUND);
                   free(ComPort);
                   // Irman found leave the constructor
                   return;
               }
           } else {
               PrintMessage((char **)&ComPort, 1, MSG_COMPORT);
           }
           // If we get here there is no Irman on this port
           Close();
       } else {
           PrintMessage((char **)&ComPort, 1, MSG_COMPRESENTUSE);
       }
   }
   // No Irman found
   free(ComPort);
   rc=(!NO_ERROR);
}

Irman::~Irman()
{
    Close();
}

void Irman::Close()
{
   rc=DosClose(hfComPort);
   if (rc!=NO_ERROR) {
       PrintMessage(NULL, 0, MSG_COMRELEASE);
   }
}

BOOL Irman::ComSetup()
{
   if (SetLineControl()==FALSE)
   {
      return FALSE;
   }
   if (SetDeviceControlBlock()==FALSE)
   {
      return FALSE;
   }
   if (SetBaudRate()==FALSE)
   {
      return FALSE;
   }
   return TRUE;
}

BOOL Irman::SetBaudRate()
{
   ULONG   ParmLen=2;
   USHORT  baud=9600;

   rc=DosDevIOCtl(hfComPort, IOCTL_ASYNC, ASYNC_SETBAUDRATE, (PULONG) &baud, sizeof(baud), &ParmLen, NULL, 0, NULL);
   if (rc!=NO_ERROR)
   {
       PrintMessage(NULL, 0, MSG_COMBAUD);
       return FALSE;
   }
   return TRUE;
}

BOOL Irman::SetLineControl()
{
   LINECTRL linectrl={0};
   ULONG   ParmLen=2;

   // Setup the LineControl
   linectrl.databits=8;
   linectrl.parity=NO_PARITY;
   linectrl.stopbits=ONE_STOPBIT;

   rc=DosDevIOCtl(hfComPort, IOCTL_ASYNC, ASYNC_SETLINECTRL, (PULONG) &linectrl, sizeof(LINECTRL), &ParmLen, NULL, 0, NULL);
   if (rc!=NO_ERROR)
      {
         PrintMessage(NULL, 0, MSG_COMLINECTRL);
         return FALSE;
      }
    return TRUE;
}

BOOL Irman::SetDeviceControlBlock()
{
   ULONG   ParmLen=2;
   DCBINFO dcb={0};

   // Setup the DeviceControlBlock
   dcb.usWriteTimeout=0;
   dcb.usReadTimeout=0;
   dcb.fbCtlHndShake=1; /* Enable DTR Control mode */
   dcb.fbFlowReplace=64; /* Enable RTS Control mode */
   dcb.fbTimeout=4; /* No Write Timeout Wait on Read Timeout */
   dcb.bXONChar=0x11;
   dcb.bXOFFChar=0x13;
   dcb.bErrorReplacementChar=0; /* Doesn't matter is disabled */
   dcb.bBreakReplacementChar=0; /* Doesn't matter is disabled */

   rc=DosDevIOCtl(hfComPort, IOCTL_ASYNC, ASYNC_SETDCBINFO, (PULONG) &dcb , sizeof(DCBINFO), &ParmLen, NULL, 0, NULL);
   if (rc!=NO_ERROR)
   {
       PrintMessage(NULL, 0, MSG_COMDCB);
       return FALSE;
   }
   return TRUE;
}

BOOL Irman::Reset()
{
   char buf[2];
   ULONG actual;

   // Give Irman time to get ready
   DosSleep(100);

   // Flush the buffers
   rc=DosDevIOCtl(hfComPort, IOCTL_GENERAL, DEV_FLUSHINPUT, NULL, 0, NULL, NULL, 0, NULL);
   if (rc!=NO_ERROR)
   {
      PrintMessage(NULL, 0, MSG_COMFLUSH);
      return FALSE;
   }
   rc=DosDevIOCtl(hfComPort, IOCTL_GENERAL, DEV_FLUSHOUTPUT, NULL, 0, NULL, NULL, 0, NULL);
   if (rc!=NO_ERROR)
   {
      PrintMessage(NULL, 0, MSG_COMFLUSH);
      return FALSE;
   }

   // Write the init string to the Irman
   rc=DosWrite(hfComPort, "IR", 2,  &actual);
   if (rc!=NO_ERROR)
   {
      PrintMessage(NULL, 0, MSG_COMREAD);
      return FALSE;
   }
   // Need a pause of at least .5 msec
   DosSleep(1);

   // Read the response of Irman
   rc=DosRead(hfComPort, buf, 2, &actual);
   if (actual==2 || rc!=NO_ERROR)
   {
      if ((rc=memcmp(buf, "OK", 2))!=0)
      {
         PrintMessage(NULL, 0, MSG_COMRESPONSE);
         return FALSE;
      }
   }
   else
   {
      PrintMessage(NULL, 0, MSG_COMRESPONSE);
      return FALSE;
   }
   return TRUE;
}

BOOL Irman::Flush()
{

   // Leeg de buffer van de com-poort(driver)
   rc=DosDevIOCtl(hfComPort, IOCTL_GENERAL, DEV_FLUSHINPUT, NULL, 0, NULL, NULL, 0, NULL);
   if (rc!=NO_ERROR)
   {
      PrintMessage(NULL, 0, MSG_COMFLUSH);
      return FALSE;
   }
   return TRUE;
}

IrData Irman::ReadData()
{
   ULONG Actual=0;
   char Buf[PACKETSIZE]={0};
   IrData irdNew;

   rc=DosRead(hfComPort, Buf, PACKETSIZE, &Actual);
   if (Actual==PACKETSIZE && (rc==NO_ERROR || rc==ERROR_MORE_DATA))
   {
      // We kijken of de data die we nu in hebben gelezen gelijk is aan die van de vorige keer
      // en of het ook nog binnen de herhaal interval is.
      irdNew=Buf;

      if (!(irdNew==irdPrevious && tRepeatDelay.IsPosted()==0))
      { // De data is nieuw
   
         // Kopieer de data naar PrevBuf om te bewaren
         irdPrevious=irdNew;

         // Reset de timer
         tRepeatDelay.Reset();

      } // if (test repetitie)
      else
         { // Het is een kopie, we geven de nulstring terug;
            irdNew="\0\0\0\0\0\0";
         }
   } else {
       if (rc!=ERROR_NO_DATA && rc!=NO_ERROR) {
           PrintMessage(NULL, 0, MSG_COMREAD);
       }
   }

return irdNew;
}

APIRET Irman::LastError()
{
    return rc;
}