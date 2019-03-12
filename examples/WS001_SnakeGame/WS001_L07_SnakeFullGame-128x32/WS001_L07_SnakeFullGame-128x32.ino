#include <PagonGameDev_GameLoop.h>

//===========================================================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"

#define OLED_RESET 4
#define BUZZER_PIN 10

Adafruit_SSD1306 display(OLED_RESET);

// This file is designed to run on a 128x32 OLED, but it would work on large ones too.
// Just change i2caddr in display.begin below to the correct value for your display
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//===========================================================

void GameLoop();
int GameFPS = 100; // 25 Enough for display but 100 for cal game speed
GameInterval GameInterval(1000 / GameFPS, GameLoop);

int iInputLeftPin = 7;
int iInputRightPin = 8;

bool isBuffInputLeft = HIGH;
bool isBuffInputRight = HIGH;

void setup()   {

  //commenting this out saves memory, uncommenting may cause the game to crash on Arduinos with limited memory like UNO R3
  //Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);

  //===========================================================
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)  // init done
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

int iCanvasWidth = 96;
int iCanvasHeight = 32;

int iGameResScale = 4;
int iGameResWidth = 24;
int iGameResHeight = 8;
bool bmpGameRes[8][24];

float fHeroPosXOrigin = 10;
float fHeroPosYOrigin = 3;

const int iHeroLengthMax = iGameResWidth * iGameResHeight;
int iHeroLengthStart = 3;
int iHeroLengthNow = iHeroLengthStart;
int iHeroLengthGlow = iHeroLengthStart;
int iHeroGlowing = 0;

int iHighScore = 0;

byte bHeroPos[24 * 8][2];
byte bByteNegative = 1;

byte iHeroState = 0;
int  iHeroDirection = 1;
int  iBuffHeroDirection = 1;

float fSpeedStart = 5;
float fSpeedScale = 2;
float fSpeedMin = 1;
float fSpeedMax = 10;
float fSpeed = fSpeedStart;

float fTimeCount;
float fTimeMax = 1 / (fSpeed * fSpeedScale);

byte bFruitPosX;
byte bFruitPosY;
byte bFruitEmplyBirth[24 * 8];
byte bFruitEmplyMax = 0;
byte bFruitRandom = 0;

void GameInit()
{
  randomSeed(analogRead(0));
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
    case 0://DisplaySpeed
      break;

    case 1://InitSpeed

      //setting screen has been replaced with a splash screen
      //note that the snake gets faster each time it eats an apple

      if (GameInput::getKeyDown(iInputRightPin, LOW, isBuffInputRight) || GameInput::getKeyDown(iInputLeftPin, LOW, isBuffInputLeft)) {
        fTimeMax = 1 / (fSpeed * fSpeedScale);
        iHeroState = 2;
      }

      break;

    case 2: //Init
      bHeroPos[0][0] = fHeroPosYOrigin + bByteNegative;
      bHeroPos[0][1] = fHeroPosXOrigin + bByteNegative;

      for (int i = 1 ; i < iHeroLengthNow ; i++)
      {
        bHeroPos[i][0] = fHeroPosYOrigin + bByteNegative;
        bHeroPos[i][1] =  bHeroPos[i - 1][1] - 1 ;

        if (bHeroPos[i][1] - bByteNegative  < 0) {
          bHeroPos[i][1] = iGameResWidth - 1 + bByteNegative;
        }
      }
      iHeroLengthNow = iHeroLengthStart;
      iHeroLengthGlow = iHeroLengthStart;
      iHeroGlowing = 0;

      iHeroDirection = 1;
      iBuffHeroDirection = 1;
      fTimeCount = 0;

      iHeroState = 3;

      fnCalCanvas();
      fnCreateFruit();
      break;

    case 3://Cal Canvas
      fnCalCanvas();
      iHeroState = 4;
      break;

    case 4: //Draw Canvas
      break;

    case 5: //Init Move
      iBuffHeroDirection = iHeroDirection;
      iHeroState = 6;
      break;

    case 6://Wait Move
      if (GameInput::getKeyDown(iInputLeftPin, LOW, isBuffInputLeft))
      {
        iBuffHeroDirection = iHeroDirection - 1;
      }

      if (GameInput::getKeyDown(iInputRightPin, LOW, isBuffInputRight))
      {
        iBuffHeroDirection = iHeroDirection + 1;
      }

      if (iBuffHeroDirection < 0) {
        iBuffHeroDirection = 3;
      }
      if (iBuffHeroDirection >= 4) {
        iBuffHeroDirection = 0;
      }

      //Speed Count
      fTimeCount += GameInterval.DeltaTime;
      if (fTimeCount >= fTimeMax)
      {
        iHeroDirection = iBuffHeroDirection;
        fTimeCount = 0;
        iHeroState = 7;
      }
      break;

    case 7: //Cal Move
      //Cal Tail
      for (int i = iHeroLengthNow - 1 ; i > 0 ; i--)
      {
        if (i != 0)
        {
          bHeroPos[i][0] =  bHeroPos[i - 1][0];
          bHeroPos[i][1] =  bHeroPos[i - 1][1];
        }
      }

      if (iHeroGlowing != 0)
      {
        iHeroLengthNow += iHeroGlowing;
        iHeroGlowing = 0;
      }

      //Cal Eaten Fruit
      for (int i = iHeroLengthNow ; i < iHeroLengthGlow ; i++)
      {
        if (bHeroPos[i][0] ==  bHeroPos[i - 1][0] && bHeroPos[i][1] ==  bHeroPos[i - 1][1])
        {
          iHeroGlowing++;

        }
      }

      //Move
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

      //Move to Border
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

      //collision detection body
      if (bmpGameRes[bHeroPos[0][0] - bByteNegative][bHeroPos[0][1] - bByteNegative] == 0)
      {
        //collision detection fruit
        if (bHeroPos[0][0] - bByteNegative == bFruitPosY && bHeroPos[0][1] - bByteNegative == bFruitPosX)
        {
          bHeroPos[iHeroLengthGlow][0] = bFruitPosY + bByteNegative;
          bHeroPos[iHeroLengthGlow][1] = bFruitPosX + bByteNegative;
          iHeroLengthGlow++;
          bmpGameRes[bHeroPos[0][0] - bByteNegative][bHeroPos[0][1] - bByteNegative] = 1;
          int note = NOTE_C1 + (iHeroLengthGlow * 5);
          tone(BUZZER_PIN, note, 50);
          fTimeMax = fTimeMax * .9;
          fnCreateFruit();
        }

        iHeroState = 8;
      }
      else {
        iHeroState = 9;
      }
      break;

    case 8:
      iHeroState = 3;
      break;

    case 9 : //GameOver
      break;

    case 10 : //Press to New Game
      if (GameInput::getKeyDown(iInputRightPin, LOW, isBuffInputRight))
      {
        iHeroState = 0;
      }
      break;
  }
}

void GameDraw()
{
  switch (iHeroState)
  {
    case 0://DisplaySpeed
      display.clearDisplay();
      display.drawRect(0, 0, display.width(), display.height(), WHITE);
      display.setTextSize(1);
      display.setCursor(0, 5);
      display.setTextColor(WHITE);
      display.print("     ");
      display.setTextColor(BLACK, WHITE);
      display.println(" S N A K E ");
      display.setTextColor(WHITE);
      display.println("Press button to Start");
      if (iHighScore > 0) {
        display.print("   High Score : ");
        display.println(iHighScore);
      }
      display.display();
      iHeroState = 1;
      break;

    case 4:
      display.clearDisplay();

      for (int i = 0 ; i < iHeroLengthNow ; i++)
      {
        display.drawBitmap( (bHeroPos[i][1] - bByteNegative) * iGameResScale , (bHeroPos[i][0] - bByteNegative) * iGameResScale , bmpHero, iBmpHeroWidth, iBmpHeroHeight , WHITE);
      }

      for (int i = iHeroLengthNow ; i < iHeroLengthGlow ; i++)
      {
        display.drawBitmap( (bHeroPos[i][1] - bByteNegative) * iGameResScale , (bHeroPos[i][0] - bByteNegative) * iGameResScale , bmpFruit, iBmpFruitWidth, iBmpFruitHeight , WHITE);
      }

      display.drawBitmap(bFruitPosX * iGameResScale , bFruitPosY * iGameResScale , bmpFruit, iBmpFruitWidth, iBmpFruitHeight , WHITE);

      //display.drawRect(0, 0, 96, display.height(), WHITE);
      display.drawLine(96, 0, 96, display.height(), WHITE);
      display.setTextColor(WHITE);
      display.setCursor(98, 0);
      display.println("Snake");
      display.drawLine(96, 10, display.width(), 10, WHITE);
      display.setCursor(98, 14);
      display.println("Score");
      display.setCursor(98, 24);
      display.print("  ");
      display.println((iHeroLengthGlow - iHeroLengthStart));
      display.display();

      iHeroState = 5;
      break;

    case 9 : //GameOver

      int score = iHeroLengthGlow - iHeroLengthStart;

      if (score > iHighScore) {
        iHighScore = score;
        playNewHighScoreEffect();
      } else {
        tone(BUZZER_PIN, NOTE_D3, 250);
        delay(350);
        tone(BUZZER_PIN, NOTE_CS3, 250);
        delay(350);
        tone(BUZZER_PIN, NOTE_F2, 1500);
      }

      display.clearDisplay();
      display.drawRect(0, 0, display.width(), display.height(), WHITE);
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println();
      display.print("    ");
      display.setTextColor(BLACK, WHITE);
      display.println("  Game Over  ");
      display.setTextColor(WHITE);
      display.print("      Score : ");
      display.println(score);
      display.display();
      iHeroState = 10;
      break;
  }
}

void playNewHighScoreEffect() {
  for (int thisNote = 0; thisNote < 2; thisNote++) {
    tone(BUZZER_PIN, NOTE_CS6, 100);
    // Output the voice after several minutes
    delay(200);
  }
  tone(BUZZER_PIN, NOTE_FS6, 200);
  delay(200);
}

void fnCreateFruit()
{
  bFruitEmplyMax = 0;
  for (int y = 0 ; y < iGameResHeight ; y++)
  {
    for (int x = 0 ; x < iGameResWidth ; x++)
    {
      if (bmpGameRes[y][x] == 0)
      {
        bFruitEmplyBirth[bFruitEmplyMax] = (y * iGameResWidth) + x;
        bFruitEmplyMax++;
      }
    }
  }
  if (bFruitEmplyMax == 0)
  {
    iHeroState = 9;
  } else {
    bFruitRandom = random(bFruitEmplyMax);
    bFruitPosX = floor(bFruitEmplyBirth[bFruitRandom] % iGameResWidth);
    bFruitPosY = floor(bFruitEmplyBirth[bFruitRandom] / iGameResWidth);
  }
}

void fnCalCanvas()
{
  for (int y = 0 ; y < iGameResHeight ; y++)
  {
    for (int x = 0 ; x < iGameResWidth ; x++)
    {
      bmpGameRes[y][x] = 0;
    }
  }

  for (int i = 0 ; i < iHeroLengthNow ; i++)
  {
    bmpGameRes[(int)bHeroPos[i][0] - bByteNegative][bHeroPos[i][1] - bByteNegative] = 1;
  }
}
