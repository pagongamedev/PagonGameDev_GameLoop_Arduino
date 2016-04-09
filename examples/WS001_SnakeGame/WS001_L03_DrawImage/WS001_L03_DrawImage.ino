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

int iInputUpPin = 12;
int iInputDownPin = 14;
int iInputLeftPin = 3;
int iInputRightPin = 1;

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

  pinMode(iInput1Pin, INPUT_PULLUP);
  pinMode(iInput2Pin, INPUT_PULLUP);

  pinMode(iInputUpPin, INPUT_PULLUP);
  pinMode(iInputDownPin, INPUT_PULLUP);
  pinMode(iInputLeftPin, INPUT_PULLUP);
  pinMode(iInputRightPin, INPUT_PULLUP);

}

void loop() {
  GameInterval.update();
}

//===========================================================

void GameLoop()
{
  GameUpdate();
  GameDraw();
}
int iCanvasWidth = 128;
int iCanvasHeight = 64;

float fHeroSpeed = 50;

float fHeroPosX = 56;
float fHeroPosY = 24;

int iHeroWidth = 16;
int iHeroHeight = 16;

static const unsigned char PROGMEM bmpHero[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

void GameUpdate()
{
  if (GameInput::getKey(iInputUpPin, LOW))
  {
    fHeroPosY -= GameInterval.DeltaTime * fHeroSpeed;
  }

  if (GameInput::getKey(iInputDownPin, LOW))
  {
    fHeroPosY += GameInterval.DeltaTime * fHeroSpeed;
  }

  if (GameInput::getKey(iInputLeftPin, LOW) || GameInput::getKey(iInput1Pin, LOW))
  {
    fHeroPosX -= GameInterval.DeltaTime * fHeroSpeed;
  }

  if (GameInput::getKey(iInputRightPin, LOW) || GameInput::getKey(iInput2Pin, LOW))
  {
    fHeroPosX += GameInterval.DeltaTime * fHeroSpeed;
  }

  //

  if (fHeroPosX < 0)
  {
    fHeroPosX = 0;
  }
  if (fHeroPosX > iCanvasWidth  - iHeroWidth )
  {
    fHeroPosX = iCanvasWidth  - iHeroWidth;
  }


  if (fHeroPosY < 0)
  {
    fHeroPosY = 0;
  }
  if (fHeroPosY > iCanvasHeight  - iHeroHeight )
  {
    fHeroPosY = iCanvasHeight  - iHeroHeight;
  }

}

void GameDraw()
{
  display.clearDisplay();

  display.drawBitmap(30, 30, bmpHero, iHeroWidth, iHeroHeight, WHITE);
  display.drawBitmap((int)fHeroPosX, (int)fHeroPosY, bmpHero, iHeroWidth, iHeroHeight, WHITE);
  display.drawBitmap(82, 30, bmpHero, iHeroWidth, iHeroHeight, BLACK);
  display.display();
}


