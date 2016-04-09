#include <PagonGameDev_GameLoop.h>

int GameFPS = 25;
void GameLoop();
GameInterval TimeInterval(1000 / GameFPS, GameLoop);

int iGameInputPin1 = 0;
int iGameInputPin2 = 13;

void setup() {
  Serial.begin(9600);

  pinMode(iGameInputPin1, INPUT_PULLUP);
  pinMode(iGameInputPin2, INPUT_PULLUP);
}

void loop() {
  TimeInterval.update();
}

float fCount = 0;
float fSpeed = 25;
void GameLoop()
{
  if (GameInput::getKey(iGameInputPin1, LOW)) //GetKey
  {
    fCount += TimeInterval.DeltaTime * fSpeed;
    Serial.println("Count : " + ((String)fCount));
  }

  if (GameInput::getKey(iGameInputPin2, LOW)) //GetKey
  {
    fCount -= TimeInterval.DeltaTime * fSpeed;
    Serial.println("Count : " + ((String)fCount));
  }
}
