#include <PagonGameDev_GameLoop.h>

void HelloWorld();
void SayHi();

GameInterval myInterval1(1000, HelloWorld);
GameInterval myInterval2(500, SayHi);


void setup() {
  Serial.begin(9600);
}

void loop() {
  myInterval1.update();
  myInterval2.update();
}

void HelloWorld()
{
  Serial.println("HelloWorld : " + ((String)(myInterval1.DeltaTime)) + " Sec.");
}

void SayHi()
{
  Serial.println("SayHi : " + ((String)(myInterval2.DeltaTime)) + " Sec.");
}


