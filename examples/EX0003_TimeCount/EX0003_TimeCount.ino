#include <PagonGameDev_GameLoop.h>

int GameFPS = 25;
void GameLoop();
GameInterval TimeInterval(1000 / GameFPS, GameLoop);

void setup() {
  Serial.begin(9600);
}

void loop() {
  TimeInterval.update();
}

float fTimeCount = 0;
void GameLoop()
{
  fTimeCount += TimeInterval.DeltaTime;
  Serial.println("TimeCount : " + ((String)fTimeCount) + " Sec.");
}
