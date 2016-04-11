#include <PagonGameDev_GameLoop.h>

int GameFPS = 25;
GameInterval TimeInterval(1000 / GameFPS, [&]() {
  Serial.println("Hello World : " + ((String)TimeInterval.DeltaTime) + " Sec.");
} );

void setup() {
  Serial.begin(9600);
}

void loop() {
  TimeInterval.update();
}
