#include <Wire.h>
#include <OBD2UART.h>
#include<Arduino.h>
#include<U8g2lib.h>



COBD obd;

#ifdef U8X8_HAVE_HW_SPI
#include<SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include<Wire.h>
#endif

//Set parameters for the screen
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//Define button and LED NANO
//const int  buttonPin = 2;    // the pin that the pushbutton is attached to
//const int ledPin = 12;       // the pin that the LED is attached to

//Define button and LED STM32
const int  buttonPin = PB3;    // the pin that the pushbutton is attached to
const int ledPin = PB1;       // the pin that the LED is attached to

// Define vars for screen rotation
int currentScreen = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

//Define the screen pages
int number_of_screens = 3;


// Define the bitmap
#define Audi_splash2_width 128
#define Audi_splash2_height 64
static const unsigned char Audi_splash2_bits[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
   0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0xc0, 0x7f, 0x00,
   0xf0, 0x0f, 0x80, 0xff, 0xff, 0x01, 0xfc, 0x03, 0x00, 0x00, 0xff, 0xff,
   0x0f, 0xc0, 0x7f, 0x00, 0xf8, 0x0f, 0xe0, 0xff, 0xff, 0x03, 0xfc, 0x07,
   0x00, 0x80, 0xff, 0xff, 0x0f, 0xc0, 0x7f, 0x00, 0xf8, 0x0f, 0xf8, 0xff,
   0xff, 0x07, 0xfc, 0x07, 0x00, 0xc0, 0xff, 0xf9, 0x1f, 0xc0, 0x7f, 0x00,
   0xf8, 0x0f, 0xfe, 0xff, 0xff, 0x07, 0xfc, 0x07, 0x00, 0xc0, 0xff, 0xf9,
   0x3f, 0xc0, 0x7f, 0x00, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0x07,
   0x00, 0xe0, 0xff, 0xf0, 0x3f, 0xc0, 0x7f, 0x00, 0xf8, 0x8f, 0xff, 0xff,
   0xff, 0x1f, 0xfc, 0x07, 0x00, 0xf0, 0x7f, 0xe0, 0x7f, 0xc0, 0x7f, 0x00,
   0xf8, 0xcf, 0xff, 0x03, 0xfc, 0x3f, 0xfc, 0x07, 0x00, 0xf0, 0x7f, 0xe0,
   0xff, 0xc0, 0x7f, 0x00, 0xf8, 0xcf, 0xff, 0x01, 0xf0, 0x3f, 0xfc, 0x07,
   0x00, 0xf8, 0x3f, 0xc0, 0xff, 0xc0, 0x7f, 0x00, 0xf8, 0xcf, 0x7f, 0x00,
   0xe0, 0x7f, 0xfc, 0x07, 0x00, 0xfc, 0x1f, 0x80, 0xff, 0xc1, 0x7f, 0x00,
   0xf8, 0xef, 0x7f, 0x00, 0xc0, 0x7f, 0xfc, 0x07, 0x00, 0xfe, 0x0f, 0x80,
   0xff, 0xc3, 0x7f, 0x00, 0xf8, 0xef, 0x3f, 0x00, 0xc0, 0x7f, 0xfc, 0x07,
   0x00, 0xfe, 0x0f, 0x00, 0xff, 0xc3, 0x7f, 0x00, 0xf8, 0xef, 0x3f, 0x00,
   0xc0, 0x7f, 0xfc, 0x07, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x7f, 0x00,
   0xf8, 0xef, 0x7f, 0x00, 0xc0, 0x7f, 0xfc, 0x07, 0x80, 0xff, 0xff, 0xff,
   0xff, 0xcf, 0xff, 0x00, 0xfc, 0xcf, 0x7f, 0x00, 0xe0, 0x7f, 0xfc, 0x07,
   0x80, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0x01, 0xfe, 0xcf, 0xff, 0x00,
   0xf0, 0x7f, 0xfc, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x07,
   0xff, 0xcf, 0xff, 0x03, 0xf8, 0x3f, 0xfc, 0x07, 0xe0, 0xff, 0xff, 0xff,
   0xff, 0xbf, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0x07,
   0xe0, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff,
   0xff, 0x0f, 0xfc, 0x07, 0xf0, 0x7f, 0x00, 0x00, 0xf0, 0x7f, 0xfe, 0xff,
   0xff, 0x03, 0xfe, 0xff, 0xff, 0x07, 0xfc, 0x07, 0xf8, 0x3f, 0x00, 0x00,
   0xe0, 0xff, 0xfc, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0x03, 0xfc, 0x07,
   0xfc, 0x1f, 0x00, 0x00, 0xc0, 0xff, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff,
   0xff, 0x00, 0xfc, 0x07, 0xfc, 0x1f, 0x00, 0x00, 0xc0, 0xff, 0xc1, 0xff,
   0x0f, 0x00, 0x80, 0xff, 0x3f, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0xff, 0xff, 0xff, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x8f, 0xff,
   0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
   0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff,
   0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff,
   0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff, 0x7f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x00,
   0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
   0x1f, 0x00, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0x00,
   0xfe, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0x07, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00,
   0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0x03, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x80,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
   0x01, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xc0,
   0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff,
   0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0xe0,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f,
   0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0xf0,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f,
   0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0x00, 0xf8,
   0x03, 0x00, 0x00, 0x00 };


//Function to draw the bitmap
void drawSplash() {
 // graphic commands to redraw the complete screen should be placed here
  u8g2.drawXBMP( 0, 0, Audi_splash2_width, Audi_splash2_height, Audi_splash2_bits);
}

//Funtion reconnect when no OBD connection
void reconnect()
{
  //Turn on Led
  digitalWrite(ledPin, HIGH);
  //Print "Reconnecting"
  u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_roentgen_nbp_t_all);
      u8g2.drawStr(0,15,"Reconnecting");
    } while ( u8g2.nextPage() );
      delay(1000);
  //digitalWrite(SD_CS_PIN, LOW);
  for (uint16_t i = 0; !obd.init(); i++) {
    if (i == 5) {
      //Turn off Led
      digitalWrite(ledPin, LOW);
      u8g2.clear();
    }
    delay(3000);
  }
}

//function to check SRAM
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

/*Text block for vars
u8g2.setFont(u8g2_font_roentgen_nbp_tr);
u8g2.setCursor(0,50);
u8g2.print(value);*/


//Draw the screen with live values
void DrawScreen(int thescreen) {
  switch (thescreen) {
    case 0: {
        static byte pidsScreen1[2] = {PID_SPEED, PID_RPM};
        int valuesScreen1[sizeof(pidsScreen1)];
        //Define vars for gauge on first page
        byte centerx=30; //x center
        byte centery=32; //y center
        byte radius=30; //radius
        byte percent=90; //needle percent
        int minVal = 0;
        int maxVal = 8000;
        int n=(radius/100.00)*percent;
        float gs_rad=-1.572;
        float ge_rad=3.141;
        if(obd.readPID(pidsScreen1, sizeof(pidsScreen1), valuesScreen1) == sizeof(pidsScreen1)) {
          // Draw gauge
          float i = ((valuesScreen1[1] - 0) * (ge_rad - gs_rad) / (maxVal - minVal) + gs_rad);
          int xp = centerx + (sin(i) * n);
          int yp = centery - (cos(i) * n);
          u8g2.firstPage();
          do {
            u8g2.drawCircle(centerx, centery, radius, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_RIGHT);
            u8g2.drawLine(centerx, centery, xp, yp);
            // we weten welke pids we gaan ophalen
            u8g2.setFont(u8g2_font_profont15_mf);
            u8g2.drawStr(64, 10, "speed");
            u8g2.drawStr(64, 30, "RPM");
            u8g2.setCursor(100, 10);
            u8g2.print((unsigned int)valuesScreen1[0] % 1000);
            u8g2.setCursor(100, 30);
            u8g2.print((unsigned int)valuesScreen1[1] % 10000);
            } while (u8g2.nextPage());
        }
      break;
    }
    case 1: {
      static byte pidsScreen2[2] = {PID_COOLANT_TEMP, PID_INTAKE_TEMP};
      int valuesScreen2[sizeof(pidsScreen2)];
        // we weten welke pids we gaan ophalen
        if(obd.readPID(pidsScreen2, sizeof(pidsScreen2), valuesScreen2) == sizeof(pidsScreen2)) {
          u8g2.firstPage();
          do {
            u8g2.setFont(u8g2_font_profont15_mf);
            u8g2.drawStr(0, 10, "Coolant temp");
            u8g2.drawStr(0, 30, "Intake temp");
            u8g2.setCursor(100, 10);
            u8g2.print(valuesScreen2[0]);
            u8g2.setCursor(100, 30);
            u8g2.print(valuesScreen2[1]);
            } while (u8g2.nextPage());
        }
      break;
    }
    case 2: {
      byte pidsScreen3[1] = {PID_THROTTLE};
      int valuesScreen3[sizeof(pidsScreen3)];
        // we weten welke pids we gaan ophalen
        if(obd.readPID(pidsScreen3, sizeof(pidsScreen3), valuesScreen3) == sizeof(pidsScreen3)) {
          u8g2.firstPage();
          do {
            u8g2.setFont(u8g2_font_profont15_mf);
            //u8g2.drawStr(0, 10, "Fuel");
            u8g2.drawStr(0, 10, "Throttle");
            u8g2.setCursor(100, 10);
            u8g2.print(valuesScreen3[0]);
            //u8g2.setCursor(100, 30);
            //u8g2.print(valuesScreen3[1]);
            } while (u8g2.nextPage());
        }
        else{
          u8g2.firstPage();
          do {
            u8g2.setFont(u8g2_font_profont15_mf);
            u8g2.drawStr(64,32, "ERROR");
            } while (u8g2.nextPage());
        }
      break;
    }
  }
}

void setup()
{
  //Init ledPin as output
  pinMode(ledPin, OUTPUT);
  //Init Screen
  u8g2.begin();
  delay(200);
   //Init OBD UART
  obd.begin();
  //Turn on Led
  digitalWrite(ledPin, HIGH);
  // Drawing the splash screen
  u8g2.firstPage();
    do {
        drawSplash();
    } while( u8g2.nextPage() );
  delay(2000);
 //Connect to OBD
  while (!obd.init());
  //Turn of ledPin
  digitalWrite(ledPin, LOW);
  //Clear screen and go to loop
  u8g2.clear();
}

void loop()
{


  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      currentScreen++;
      currentScreen = currentScreen % number_of_screens;
      // Delay a little bit to avoid bouncing
      delay(100);
      u8g2.clear();
    }
  }
// save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  //Draw the screens
  DrawScreen(currentScreen);

//Reconnect if no connection
  if (obd.errors >= 2) {
      delay(2000);
      reconnect();
      setup();
  }
}
