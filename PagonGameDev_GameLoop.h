//================================================
// Dev by : Pagon Game Dev
// PagonGameDev@gmail.com
//================================================

#ifndef PagonGameDev_GameLoop_h
#define PagonGameDev_GameLoop_h

#include "Arduino.h"

class GameInterval
{
  public:
    GameInterval(int iInputInterval, void (funcInterval)());
    void update();
    float DeltaTime;


  private:
    int iMillisInterval;
    int iMillisDelayTime;
    int iMillisIntervalDelayTime;

    unsigned long lMillisPrevious;
    unsigned long lMillisCurrent;
    void(*functionInterval)();
};

class GameInput
{
  public:
    GameInput();
    static bool getKey(int iPinInput, bool isTrueState);
    static bool getKeyDown(int iPinInput, bool isTrueState, bool& bInputBuffRead);
    static bool getKeyUp(int iPinInput, bool isTrueState, bool& bInputBuffRead);
};
#endif
