#include <PagonGameDev_GameLoop.h>

int GameFPS = 25;
void GameLoop();
GameInterval FPSInterval(1000 / GameFPS, GameLoop);

void setup() {
  Serial.begin(9600);
}

void loop() {
  FPSInterval.update();
}

void GameLoop()
{
  Serial.print("FPS : ");
  Serial.println((1 / FPSInterval.DeltaTime));
}
