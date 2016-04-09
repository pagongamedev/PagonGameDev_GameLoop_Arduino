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

void CalFPS();
void CalTime();

int GameFPS = 60;
GameInterval FPSInterval(1000 / GameFPS, CalFPS);
GameInterval TimeInterval(1000, CalTime);

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
}

void loop() {
  FPSInterval.update();
  TimeInterval.update();
}

int iTimeCount;
int iFPSCount;

int iFPSPerTime;
float fTimeFromDelta;

void CalFPS()
{
  fTimeFromDelta +=  FPSInterval.DeltaTime;
  iFPSCount++;

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.print("FPS " + ((String)GameFPS) + " ");
  display.setTextColor(WHITE);
  display.print(" ");
  display.println((1 / FPSInterval.DeltaTime));

  display.println();
  display.setTextColor(BLACK, WHITE);
  display.println("Time");
  display.setTextColor(WHITE);
  display.println();
  display.println("Sec    : " + ((String)iTimeCount));
  display.println("FPS    : " + ((String)iFPSPerTime));
  display.print("D Time : ");
  display.println(FPSInterval.DeltaTime, 3);
  display.println("D Sec  : " + ((String)fTimeFromDelta));

  display.display();
}

void CalTime()
{
  if (iTimeCount == 0)
  {
    fTimeFromDelta = 1;
  }
  iFPSPerTime = iFPSCount;
  iFPSCount = 0;
  iTimeCount++;
}
