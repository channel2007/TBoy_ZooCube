#ifndef _CLOCK_SYSTEM_H
#define _CLOCK_SYSTEM_H

class ClockSystem
{
public:
  ClockSystem() {
    initClock();
  }

  void initClock(byte n = 255) {
    if (n == 255) {
      for (byte i = 0; i < (sizeof(clockSys) / sizeof(unsigned long)); i++)
        clockSys[i] = millis();
    }
    else {
      clockSys[n] = millis();
    }
  }

  bool checkClock(byte id, unsigned long t) {
    unsigned long diff = millis() - clockSys[id];
    if (diff > t) {
      //clockSys[id] = millis() - (diff - t);
      clockSys[id] = millis();
      return true;
    }
    return false;
  }

private:  
  unsigned long clockSys[9];

};
#endif
