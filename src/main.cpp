#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
// #include <Adafruit_GFX.h>

#define ONE_WIRE_BUS 5 // Thermometer Pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

volatile boolean TurnDetected;
volatile boolean up;

const int PinCLK=2;                   // Used for generating interrupts using CLK signal
const int PinDT=3;                    // Used for reading DT signal
const int PinSW=4;                    // Used for the push button switch

float dTemp = 130;
float temp;

void isr ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
 if (digitalRead(PinCLK))
   up = digitalRead(PinDT);
 else
   up = !digitalRead(PinDT);
 TurnDetected = true;
}


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM logo_bmp[] =
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
    B00000000, B00110000 };

void updatetemps(float desiredTemp,float actualTemp) {
    display.clearDisplay();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Desired "));
    display.setCursor(64,0);
    display.println(F("Actual"));

    display.setCursor(0,18);
    display.setTextSize(2);             // Draw 3X-scale text
    display.setTextColor(WHITE);
    display.print(desiredTemp , 0);

    display.setCursor(64,18);
    display.setTextSize(2);             // Draw 3X-scale text
    display.setTextColor(WHITE);
    display.print(actualTemp, 1);

    display.display();
    delay(20);
    }

void setup() {
    pinMode(PinCLK,INPUT);
    pinMode(PinDT,INPUT);
    pinMode(PinSW,INPUT);
    attachInterrupt (0,isr,FALLING);
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    }
}

void loop() {
    sensors.requestTemperatures();

    updatetemps(dTemp , sensors.getTempFByIndex(0));

    static long virtualPosition=0;    // without STATIC it does not count correctly!!!

    if (!(digitalRead(PinSW))) {      // check if pushbutton is pressed
        virtualPosition=0;              // if YES, then reset counter to ZERO
        Serial.print ("Reset = ");      // Using the word RESET instead of COUNT here to find out a buggy encoder

        Serial.println (virtualPosition);
        }

    if (TurnDetected)  {		    // do this only if rotation was detected
        if (up)
            dTemp --;
        else
            dTemp ++;
        Serial.print(dTemp);
        TurnDetected = false;          // do NOT repeat IF loop until new rotation detected
        delay(75);
    }

}
