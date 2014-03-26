#ifndef IRMAN_HPP
#define IRMAN_HPP

// Defines voor ontbrekende delen uit os2.h
#define NO_PARITY 0
#define ODD_PARITY 1
#define EVEN_PARITY 2
#define MARK_PARITY 3
#define SPACE_PARITY 4

#define ONE_STOPBIT 0
#define ONEANDAHALF_STOPBIT 1
#define TWO_STOPBIT 2

typedef struct _LINECTRL
{
   UCHAR databits, parity, stopbits;
} LINECTRL, *PLINECTRL;

// Defines voor Irman Data
#define REPEATDELAY 500
#define PACKETSIZE 6

class Irman
{
   public:
      Irman(char *port="COM1");
      ~Irman();
      BOOL Reset();
      class IrData ReadData();
      BOOL Flush();
      APIRET LastError();
   private:
      // Functiedeclaraties
      void Close();
      BOOL ComSetup();
      BOOL SetBaudRate();
      BOOL SetLineControl();
      BOOL SetDeviceControlBlock();

      // Variabelendeclaraties
      HFILE hfComPort;
      IrData irdPrevious;
      Timer tRepeatDelay;
      APIRET rc;
};

#endif
