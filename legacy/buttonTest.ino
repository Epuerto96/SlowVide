#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



const int  buttonPin = 9;    // the pin that the Up pushbutton is attached to

const int  buttonPin1 = 8;    // the pin that the Down pushbutton is attached to


// Variables will change:


int buttonPushCounter = 102;   // counter for the number of button presses
int buttonState5 = 0;         // current state of the button
int buttonState6 = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


void setup() {


  // initialize the button pin as a input:
   
   pinMode(buttonPin, INPUT);
   
   pinMode(buttonPin1, INPUT);
   
   
   lcd.begin(16,2);
   
   lcd.setCursor(0,1);
   
   lcd.print("Volume:");
   
}


void loop() {


   // read the pushbutton up input pin:
   
   buttonState5 = digitalRead(buttonPin);

  // compare the buttonState to its previous state

   if (buttonState5 != lastButtonState) {
     if (buttonState5 == HIGH)
     {
      buttonPushCounter++;
      lcd.setCursor(7,1);       
      lcd.print(buttonPushCounter);
      delay(250);
     }
   }
   
   lastButtonState = buttonState5;
   buttonState6 = digitalRead(buttonPin1);



   if (buttonState6 != lastButtonState) {
     
     // if the state has changed, decrement the counter
     
     if (buttonState6 == HIGH)
     
     {
       if (buttonPushCounter >= 102){
      buttonPushCounter--;
      lcd.setCursor(7,1);
          
      lcd.print(buttonPushCounter);
      delay(250);}
     }
     
             
   }
   // save the current state as the last state,
  //for next time through the loop
   lastButtonState = buttonState6; 
   
   
}

