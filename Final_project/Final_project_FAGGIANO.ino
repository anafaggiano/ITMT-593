// FINAL PROJECT FAGGIANO Anaïs

*/


int hellodisplay=false;
int stage=1;
int goal=10; // goal : number of steps to reach 


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 10, en = 9, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//END OF LED CONFIG

//BUTTON CONFIG
int button=16;
int buttonState;
//END OF BUTTON CONFIG

//BLUETOOTH CONFIG
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

 #define FACTORYRESET_ENABLE         1
 #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
 #define MODE_LED_BEHAVIOUR          "MODE"

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
//END OF BLUETOOTH CONFIG

#include <SPI.h>

// Time variables
unsigned long Time ;

//Tilt Sensor variables
const int ledPin = A3;//the led attach to
const int Tilt=2;
int steps=1;
int switchState = 1;
int prevSwitchState = 0;
int displayedSteps;
//



void setup() {
  //BUTTON SETUP
  pinMode(button,INPUT);
  //END OF BUTTON SETUP
  
  //LCD SETUP
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hi! Please conne");
  lcd.setCursor(0,1);
  lcd.print("ct to bluetooth");
  //END OF LCD SETUP
  
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

/*  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
 /*   Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  
  }*/

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }
//END OF BLUETOOTH SETUP

//TILT SENSOR SETUP
pinMode(ledPin,OUTPUT);//initialize the ledPin as an output
pinMode(Tilt,INPUT);//set Tilt sensor as INPUT
digitalWrite(Tilt, HIGH);//set Tilt sensor as HIGH
digitalWrite(ledPin, HIGH);
//END OF TILT SENSOR SETUP

}//END OF SETUP
 
void loop() {
buttonState=digitalRead(button);

if(stage==1){// STAGE1 = beginning

  if(hellodisplay==false){
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Today's goal:");
lcd.setCursor(0,1);
lcd.print(objective);
lcd.print(" steps");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
  lcd.print("Press button");
  lcd.setCursor(0,1);
  lcd.print("to start walking");

  hellodisplay=true;
    }
  if (buttonState == HIGH) {
      lcd.clear();
      lcd.print("Go!");
      delay(1000);
      stage = 2;
    }//end if buttonState HIGH
    


    
}//end of stage=1

if (stage==2){ // STAGE 2 = COUNTING STEPS
//Step counting
int digitalVal = digitalRead(Tilt);//Read the value of pin2

if (switchState != prevSwitchState) {
if(HIGH == digitalVal)//if tilt switch is not breakover
{
digitalWrite(ledPin,LOW);//turn the led off
steps=steps+1;
displayedSteps=steps/100;

//LCD DISPLAY
lcd.clear();
lcd.setCursor(0,0);
lcd.print("You walked : ");
 // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print(displayedSteps);
  lcd.print(" steps");
//END OF LCD DISPLAY
//BLUETOOTH DISPLAY
Serial.println(displayedSteps);
ble.print(F("AT+BLEUARTTX="));
ble.print(F("Steps="));
ble.println(displayedSteps); 
//END OF BLUETOOTH DISPLAY

if (displayedSteps==objective){
  stage=3;
  }
}
else ////if tilt switch breakover
{
digitalWrite(ledPin,HIGH);//turn the led on
}
}//end if si etats differents

//END OF STEP COUNTING
}//end of if stage=2

if (stage==3){ //STEP3 = WIN
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Congratulations!");
  lcd.setCursor(0,1);
  lcd.println(F("You did great"));
  ble.println(F("Congratulations!"));
  delay(4000);
  stage=1;
  
  }
  
  }
