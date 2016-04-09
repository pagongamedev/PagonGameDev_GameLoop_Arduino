#include <PagonGameDev_GameLoop.h>

//===========================================================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Espert_Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//===========================================================

//

void GameLoop();
int GameFPS = 25;
GameInterval GameInterval(1000 / GameFPS, GameLoop);

int iInput1Pin = 0;
int iInput2Pin = 13;

int iInputLeftPin = 3;
int iInputRightPin = 1;

bool isBuffInput1 = HIGH;
bool isBuffInput2 = HIGH;
bool isBuffInputLeft = HIGH;
bool isBuffInputRight = HIGH;

//

void setup()   {
  Serial.begin(9600);

  //===========================================================
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x78 >> 1);
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  display.clearDisplay();
  //===========================================================

  GameInit();
}

void loop() {
  GameInterval.update();
}

//===========================================================

int iBmpHeroWidth = 4;
int iBmpHeroHeight = 4;
static const unsigned char PROGMEM bmpHero[] =
{ B01100000,
  B10010000,
  B10010000,
  B01100000,
};

int iBmpFruitWidth = 4;
int iBmpFruitHeight = 4;
static const unsigned char PROGMEM bmpFruit[] =
{ B00000000,
  B01100000,
  B01100000,
  B00000000,
};

int iCanvasWidth = 128;
int iCanvasHeight = 64;

int iGameResScale = 4;
int iGameResWidth = 32;
int iGameResHeight = 16;
bool bmpGameRes[16][32];

float fHeroPosXOrigin = 15;
float fHeroPosYOrigin = 7;

int iHeroLength = 15;
byte bHeroPos[15][2];
byte bByteNegative = 1;

byte iHeroState = 0;
int  iHeroDirection = 1;
int  iBuffHeroDirection = 1;

float fTimeCount;
float fTimeMax = 1 / 5;

byte bFruitPosX;
byte bFruitPosY;


void GameInit()
{
  pinMode(iInput1Pin, INPUT_PULLUP);
  pinMode(iInput2Pin, INPUT_PULLUP);

  pinMode(iInputLeftPin, INPUT_PULLUP);
  pinMode(iInputRightPin, INPUT_PULLUP);

}
void GameLoop()
{
  GameUpdate();
  GameDraw();
}

void GameUpdate()
{
  switch (iHeroState)
  {
    case 0:
      bHeroPos[0][0] = fHeroPosYOrigin + bByteNegative;
      bHeroPos[0][1] = fHeroPosXOrigin + bByteNegative;

      for (int i = 1 ; i < iHeroLength ; i++)
      {
        bHeroPos[i][0] = fHeroPosYOrigin + bByteNegative;
        bHeroPos[i][1] =  bHeroPos[i - 1][1] - 1 ;

        if (bHeroPos[i][1] - bByteNegative  < 0) {
          bHeroPos[i][1] = iGameResWidth - 1 + bByteNegative;
        }
      }

      bFruitPosX = random(iGameResWidth);
      bFruitPosY = random(iGameResHeight);
      iHeroState = 1;
      break;

    case 1:
      for (int y = 0 ; y < iGameResHeight ; y++)
      {
        for (int x = 0 ; x < iGameResWidth ; x++)
        {
          bmpGameRes[y][x] = 0;

        }
      }

      for (int i = 0 ; i < iHeroLength ; i++)
      {
        bmpGameRes[(int)bHeroPos[i][0] - bByteNegative][bHeroPos[i][1] - bByteNegative] = 1;
      }
      iHeroState = 2;
      break;

    case 2:
      break;

    case 3:
      iBuffHeroDirection = iHeroDirection;
      iHeroState = 4;
      break;

    case 4:
      if (GameInput::getKeyDown(iInputLeftPin, LOW, isBuffInputLeft) || GameInput::getKeyDown(iInput1Pin, LOW , isBuffInput1))
      {
        iBuffHeroDirection = iHeroDirection - 1;
      }

      if (GameInput::getKeyDown(iInputRightPin, LOW, isBuffInputRight) || GameInput::getKeyDown(iInput2Pin, LOW, isBuffInput2))
      {
        iBuffHeroDirection = iHeroDirection + 1;
      }

      if (iBuffHeroDirection < 0) {
        iBuffHeroDirection = 3;
      }
      if (iBuffHeroDirection >= 4) {
        iBuffHeroDirection = 0;
      }

      fTimeCount += GameInterval.DeltaTime;
      if (fTimeCount >= fTimeMax)
      {
        iHeroDirection = iBuffHeroDirection;
        fTimeCount = 0;
        iHeroState = 5;
      }
      break;

    case 5:
      for (int i = iHeroLength - 1 ; i > 0 ; i--)
      {
        if (i != 0)
        {
          bHeroPos[i][0] =  bHeroPos[i - 1][0];
          bHeroPos[i][1] =  bHeroPos[i - 1][1];
        }
      }

      switch (iHeroDirection)
      {
        case 0:
          bHeroPos[0][0] -= 1;
          break;

        case 1:
          bHeroPos[0][1] += 1;
          break;

        case 2:
          bHeroPos[0][0] += 1;
          break;

        case 3:
          bHeroPos[0][1] -= 1;
          break;
      }

      if (  bHeroPos[0][1] - bByteNegative < 0 )
      {
        bHeroPos[0][1]  = iGameResWidth - 1 + bByteNegative;
      }
      if (bHeroPos[0][1] - bByteNegative  >= iGameResWidth)
      {
        bHeroPos[0][1]  = 0 + bByteNegative;
      }

      if (  bHeroPos[0][0] - bByteNegative < 0 )
      {
        bHeroPos[0][0] = iGameResHeight - 1 + bByteNegative;
      }
      if (  bHeroPos[0][0] - bByteNegative >= iGameResHeight )
      {
        bHeroPos[0][0] = 0 + bByteNegative;
      }

      if (bHeroPos[0][0] - bByteNegative == bFruitPosY && bHeroPos[0][1] - bByteNegative == bFruitPosX)
      {
        bFruitPosX = random(iGameResWidth);
        bFruitPosY = random(iGameResHeight);
      }
      iHeroState = 6;
      break;

    case 6:
      iHeroState = 1;
      break;
  }
}

void GameDraw()
{
  switch (iHeroState)
  {
    case 2:
      display.clearDisplay();

      for (int y = 0 ; y < iGameResHeight ; y++)
      {
        for (int x = 0 ; x < iGameResWidth ; x++)
        {

          if (bmpGameRes[y][x] == 1)
          {
            display.drawBitmap(x * iGameResScale , y * iGameResScale , bmpHero, iBmpHeroWidth, iBmpHeroHeight , WHITE);
          }
        }
      }

      display.drawBitmap(bFruitPosX * iGameResScale , bFruitPosY * iGameResScale , bmpFruit, iBmpFruitWidth, iBmpFruitHeight , WHITE);

      display.display();

      iHeroState = 3;
      break;
  }
}
