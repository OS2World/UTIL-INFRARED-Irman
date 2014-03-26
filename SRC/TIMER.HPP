#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
   public:
      Timer(ULONG ulMyInterval=250);
      ~Timer();
      BOOL Reset();
      BOOL Stop();
      BOOL Start();
      ULONG IsPosted();
      BOOL Wait(ULONG ulTimeOut=SEM_INDEFINITE_WAIT);
      ULONG SetInterval(ULONG ulMyInterval);
      APIRET LastError();
   private:
      ULONG ulInterval;
      HEV hevSemaphore;
      HTIMER htTimer;
      APIRET rc;
};

#endif
