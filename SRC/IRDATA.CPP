// IrData Class

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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "irdata.hpp"

BOOL IrData::operator==(const IrData& ird)
{
   if (memcmp(this->pcData, ird.pcData, IRDATA_LEN)==0) return TRUE;
   else return FALSE;
}

BOOL IrData::operator!=(const IrData& ird)
{
   if (memcmp(this->pcData, ird.pcData, IRDATA_LEN)==0) return FALSE;
   else return TRUE;
}

IrData& IrData::operator=(const IrData& ird)
{
   memcpy((void *)this->pcData, (void *)ird.pcData, IRDATA_LEN);
   return (IrData&)this;
}

IrData& IrData::operator=(char *strData)
{
   memcpy(this->pcData, strData, IRDATA_LEN);
   return (IrData&)this;
}

char *IrData::GetpChar()
{
  return this->pcData;
}

char *IrData::GetXString()
{
    char *XString;
    if ((XString=(char *)malloc(IRDATA_LEN*2+1))==NULL) return NULL;
    for (int i=0; i<IRDATA_LEN; i++) {
        sprintf(XString+(2*i), "%02x", pcData[i]);
    }
    return XString;
}

IrData::IrData(const IrData& ird)
{
   memcpy(this->pcData, ird.pcData, IRDATA_LEN);
}

IrData::IrData(char *strData)
{
   memcpy(this->pcData, strData, IRDATA_LEN);
}

IrData::IrData()
{
   memset(pcData, 0, IRDATA_LEN);
}

void IrData::Print(FILE *f)
{
   for (int i=0; i<IRDATA_LEN; i++) {
      fprintf(f, "%02x", pcData[i]);
   } /* endfor */
}
    
IrData& IrData::Read(FILE *f)
{
    for (int i=0; i<IRDATA_LEN; i++) {
        fscanf(f, "%02x", pcData[i]);
    }
    return (IrData&)this;
}
