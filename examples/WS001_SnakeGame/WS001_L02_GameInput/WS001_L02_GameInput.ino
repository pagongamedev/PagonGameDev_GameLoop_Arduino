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

bool isBuffInput1 = HIGH;
bool isBuffInput2 = HIGH;

String sMode[] = {"GetKey", "GetKeyDown", "GetKeyUp"};
String sBtn[] = {"Off", "On"};
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

byte bModeState = 0;
byte bBtnState = 0;

void GameUpdate()
{

  switch (bModeState)
  {
    case 0 :

      if (GameInput::getKeyDown(iInput1Pin, LOW, isBuffInput1))
      {
        bModeState = 1;
        bBtnState = 0;
        isBuffInput2 = HIGH;
      }

      //
      if (GameInput::getKey(iInput2Pin, LOW))
      {
        bBtnState = 1;
      } else {
        bBtnState = 0;
      }

      break;

    case 1 :
      if (GameInput::getKeyDown(iInput1Pin, LOW, isBuffInput1))
      {
        bModeState = 2;
        bBtnState = 0;
        isBuffInput2 = HIGH;
      }

      //
      if (GameInput::getKeyDown(iInput2Pin, LOW, isBuffInput2))
      {
        if (bBtnState == 0) {
          bBtnState = 1;
        } else if (bBtnState == 1) {
          bBtnState = 0;
        }
      }
      break;
    case 2 :
      if (GameInput::getKeyDown(iInput1Pin, LOW, isBuffInput1))
      {
        bModeState = 0;
        bBtnState = 0;
        isBuffInput2 = HIGH;
      }

      //
      if (GameInput::getKeyUp(iInput2Pin, LOW, isBuffInput2))
      {
        if (bBtnState == 0) {
          bBtnState = 1;
        } else if (bBtnState == 1) {
          bBtnState = 0;
        }
      }
      break;
  }
}

void GameDraw()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.println("Button Mode");
  display.setTextColor(WHITE);
  display.println(" " + ((String)sMode[bModeState]));
  display.println();
  display.setTextColor(BLACK, WHITE);
  display.println("Button Action");
  display.setTextColor(WHITE);
  display.println(" " + ((String)sBtn[bBtnState]));

  display.display();
}


