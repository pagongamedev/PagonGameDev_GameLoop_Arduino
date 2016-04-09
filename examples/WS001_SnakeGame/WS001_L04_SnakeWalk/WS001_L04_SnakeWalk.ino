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
int iHeroWidth = 4;
int iHeroHeight = 4;

static const unsigned char PROGMEM bmpHero[] =
{ B01100000,
  B10010000,
  B10010000,
  B01100000,
};

int iCanvasWidth = 128;
int iCanvasHeight = 64;

int iGameResWidth = 32;
int iGameResHeight = 16;
bool bmpGameRes[16][32];

float fHeroSpeed = 5;

float fHeroPosX = 15;
float fHeroPosY = 7;

byte iHeroState = 0;
int  iHeroDirection = 1;
int  iBuffHeroDirection = 1;

float fTimeCount;
float fTimeMax = 1 / 5;

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
      // iHeroState = 1;
      break;

    case 1:
      iBuffHeroDirection = iHeroDirection;
      iHeroState = 2;
      break;

    case 2:
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
        iHeroState = 3;
      }
      break;

    case 3:
      switch (iHeroDirection)
      {
        case 0:
          fHeroPosY -= 1;
          break;

        case 1:
          fHeroPosX += 1;
          break;

        case 2:
          fHeroPosY += 1;
          break;

        case 3:
          fHeroPosX -= 1;
          break;
      }

      if (fHeroPosX < 0)
      {
        fHeroPosX = iGameResWidth - 1;
      }
      if (fHeroPosX >= iGameResWidth )
      {
        fHeroPosX = 0;
      }

      if (fHeroPosY < 0)
      {
        fHeroPosY = iGameResHeight - 1;
      }
      if (fHeroPosY >= iGameResHeight )
      {
        fHeroPosY = 0;
      }

      iHeroState = 4;
      break;

    case 4:
      for (int y = 0 ; y < iGameResHeight ; y++)
      {
        for (int x = 0 ; x < iGameResWidth ; x++)
        {
          bmpGameRes[y][x] = 0;

        }
      }

      bmpGameRes[(int)fHeroPosY][(int)fHeroPosX] = 1;
      iHeroState = 0;
      break;
  }
}

int iGameResScale = 4;
void GameDraw()
{
  switch (iHeroState)
  {
    case 0:
      display.clearDisplay();

      for (int y = 0 ; y < iGameResHeight ; y++)
      {
        for (int x = 0 ; x < iGameResWidth ; x++)
        {

          if (bmpGameRes[y][x] == 1)
          {
            display.drawBitmap(x * iGameResScale , y * iGameResScale , bmpHero, iHeroWidth, iHeroHeight , WHITE);
          }
        }
      }
      display.display();
      iHeroState = 1;
      break;
  }
}
