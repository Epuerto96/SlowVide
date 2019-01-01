#include <OneWire.h>
#include <LiquidCrystal.h>


// http://milesburton.com/Dallas_Temperature_Control_Library

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)

const int  buttonPin = 9;    // the pin that the Up pushbutton is attached to
const int  buttonPin1 = 8;    // the pin that the Down pushbutton is attached to

int buttonPushCounter = 131;   // counter for the number of button presses
int buttonState5 = 0;         // current state of the button
int buttonState6 = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int Relay = 7; 

void setup(void) {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(Relay, OUTPUT);
  lcd.begin(16, 2);
}

void loop(void) {
 
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  buttonState5 = digitalRead(buttonPin);
  buttonState6 = digitalRead(buttonPin1);
  
  if ( !ds.search(addr)) {
    ds.reset_search();

    return;
  }
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(500);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad


  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();

  }


  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  
  
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;


  lcd.print("Set Temp:");
   if (buttonState5 != lastButtonState) {
     if (buttonState5 == HIGH)
     {
      buttonPushCounter++;
     }
   }
   
   lastButtonState = buttonState5;
   buttonState6 = digitalRead(buttonPin1);



   if (buttonState6 != lastButtonState) {
     if (buttonState6 == HIGH) 
     {
       if (buttonPushCounter >= 102){
          buttonPushCounter--;
          }
     }
     
             
   }

   lastButtonState = buttonState6; 

  if((fahrenheit) < buttonPushCounter - 2){
    digitalWrite(Relay, LOW); 
  }
  else{
    digitalWrite(Relay, HIGH);
    }

  lcd.setCursor(9, 0);
  lcd.print(buttonPushCounter);
  lcd.setCursor(0, 1);
  lcd.print("Cur. Temp:");
  lcd.print(fahrenheit);
  lcd.print(" ");
  





      
   }
