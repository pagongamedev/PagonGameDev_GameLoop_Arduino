#include <PagonGameDev_GameLoop.h>

int GameFPS = 25;
void GameLoop();
GameInterval TimeInterval(1000 / GameFPS, GameLoop);

int iGameInputPin1 = 0;
int iGameInputPin2 = 13;

bool isBuffGameInputPin1 = HIGH;
bool isBuffGameInputPin2 = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(iGameInputPin1, INPUT_PULLUP);
  pinMode(iGameInputPin2, INPUT_PULLUP);
}

void loop() {
  TimeInterval.update();
}

int iCount = 0;
void GameLoop()
{
  if (GameInput::getKeyDown(iGameInputPin1, LOW, isBuffGameInputPin1)) //GetKeyDown
  {
    iCount += 1;
    Serial.println("Count : " + ((String)iCount) + " : [GetKeyDown]");
  }

  if (GameInput::getKeyUp(iGameInputPin2, LOW, isBuffGameInputPin2)) //GetKeyUp
  {
    iCount -= 1;
    Serial.println("Count : " + ((String)iCount) + " : [GetKeyUp]");
  }
}
