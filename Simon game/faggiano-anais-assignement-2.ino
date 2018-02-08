// include the library code for LCD:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Green analog input constants
int fsrAnalogGreen = A0; // FSR is connected to analog 0
int ledGreen = 9;
int fsrReadingGreen;     // the analog reading from the FSR resistor divider
int LEDbrightnessGreen;

// Red analog input constants
int fsrAnalogRed = A1; // FSR is connected to analog 1
int ledRed = 10;
int fsrReadingRed;     // the analog reading from the FSR resistor divider
int LEDbrightnessRed;

// Blue analog input constants
int fsrAnalogBlue = A2; // FSR is connected to analog 2
int ledBlue = 11;
int fsrReadingBlue;     // the analog reading from the FSR resistor divider
int LEDbrightnessBlue;

// White analog input constants
int fsrAnalogWhite = A3; // FSR is connected to analog 3
int ledWhite = 12;
int fsrReadingWhite;     // the analog reading from the FSR resistor divider
int LEDbrightnessWhite;

//FSR threshold
int FSRthreshold = 20;

//start loop contants
bool start = true;
const int buttonStart =  2;
int buttonStartState = 0;

//buzzer constants
const int buzzer = 18;
const int buzzerRed = 250;
const int buzzerGreen = 400;
const int buzzerBlue = 700;
const int buzzerWhite = 900;

//led state variables
int GreenLedOn = 0;
int RedLedOn = 0;
int BlueLedOn = 0;
int WhiteLedOn = 0;


int gameStage = 1; //1=beginning, 2=memorize, 3=play, 4=loss
String answerString = "";
String stringYourAnswer = "";
int x;

bool hellodisplay = false;

//game parameters
int gamespeed = 500;
int welcomespeed = 250;

void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor

  //LEDs setup
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledWhite, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();

  //buzzer setup
  pinMode(buttonStart, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop(void) {

  buttonStartState = digitalRead(buttonStart);
  //  Serial.print("Game stage=");
  //  Serial.println(gameStage);
  //  Serial.println(buttonStartState);

  //BEGINNING

  if (gameStage == 1) {

    if (hellodisplay == false) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello !");
      digitalWrite(ledGreen, HIGH);
      delay(welcomespeed);
      digitalWrite(ledRed, HIGH);
      delay(welcomespeed);
      digitalWrite(ledBlue, HIGH);
      delay(welcomespeed);
      digitalWrite(ledWhite, HIGH);
      delay(welcomespeed);
      digitalWrite(ledGreen, HIGH);
      delay(welcomespeed);
      digitalWrite(ledRed, HIGH);
      delay(welcomespeed);
      digitalWrite(ledBlue, HIGH);
      delay(welcomespeed);
      digitalWrite(ledWhite, HIGH);
      delay(welcomespeed);
      digitalWrite(ledRed, LOW);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledWhite, LOW);
      digitalWrite(ledBlue, LOW);
      delay(80);
      delay(2000);
      lcd.clear();
      lcd.print("Press button to");
      lcd.setCursor(0, 1);
      lcd.print("start a new game");
      delay(1000);

      hellodisplay = true;
    }//end if hellodisplay

    if (buttonStartState == HIGH) {
      lcd.clear();
      lcd.print("Ready ? ...");
      delay(3000);
      lcd.print(" Go!");
      delay(1000);
      gameStage = 2;
    }//end if buttonStartState


  }//end if gameStage=1

  //MEMORIZE
  if (gameStage == 2) {
    //randomArray

    lcd.clear();
    lcd.print("Memorize...");

    delay(1000);

    answerString += random(1, 5);

    for (x = 0; x < answerString.length(); x++) { //led lighting loop
      if (answerString.charAt(x) == '1') { //1 in the answer array is the green led
        digitalWrite(ledGreen, HIGH);
        tone(buzzer, buzzerGreen);
        delay(500);
        noTone(buzzer);
        digitalWrite(ledGreen, LOW);
        delay(gamespeed);
      }//end if 1
      if (answerString.charAt(x) == '2') { //2 in the answer array is the red led
        digitalWrite(ledRed, HIGH);
        tone(buzzer, buzzerRed);
        delay(500);
        noTone(buzzer);
        digitalWrite(ledRed, LOW);
        delay(gamespeed);
      }//end if 2
      if (answerString.charAt(x) == '3') { //3 in the answer array is the blue led
        digitalWrite(ledBlue, HIGH);
        tone(buzzer, buzzerBlue);
        delay(500);
        noTone(buzzer);
        digitalWrite(ledBlue, LOW);
        delay(gamespeed);
      }//end if 3
      if (answerString.charAt(x) == '4') { //4 in the answer array is the white led
        digitalWrite(ledWhite, HIGH);
        tone(buzzer, buzzerWhite);
        delay(500);
        noTone(buzzer);
        digitalWrite(ledWhite, LOW);
        delay(gamespeed);
      }//end if 4
    }//end for

    stringYourAnswer = "";
    gameStage = 3;
  }

  //PLAY
  if (gameStage == 3) {
    delay(1000);
    lcd.clear();
    lcd.print("Your turn");

    if (GreenLedOn) {
      stringYourAnswer += "1";
      GreenLedOn = 0;
    }//end green

    if (RedLedOn) {
      stringYourAnswer += "2";
      RedLedOn = 0;

    }

    if (BlueLedOn) {
      stringYourAnswer += "3";
      BlueLedOn = 0;

    }

    if (WhiteLedOn) {
      stringYourAnswer += "4";
      WhiteLedOn = 0;

    }

    for (int y = 0; y < stringYourAnswer.length(); y++) {
      if (stringYourAnswer.charAt(y) != answerString.charAt(y)) { //game over as soon as there is one mistake
        gameStage = 4;
      }
    }
    //    Serial.print("answerString=");
    //    Serial.println(answerString);
    //    Serial.print("stringYourAnswer=");
    //    Serial.println(stringYourAnswer);

    if (stringYourAnswer.length() == answerString.length()) {
      gameStage = 2;
    }
  }

  if (gameStage == 4) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game over");
    lcd.setCursor(0, 1);
    lcd.print("Score :");
    lcd.print(stringYourAnswer.length());

  }


  // GREEN LED
  fsrReadingGreen = analogRead(fsrAnalogGreen);
  //  Serial.print("Green reading = ");
  //  Serial.println(fsrReadingGreen);
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  LEDbrightnessGreen = map(fsrReadingGreen, 0, 1023, 0, 255);
  // LED gets brighter the harder you press

  if (fsrReadingGreen > FSRthreshold) {
    tone(buzzer, buzzerGreen);
    delay(500);
    noTone(buzzer);
    analogWrite(ledGreen, LEDbrightnessGreen);
    analogWrite(ledGreen, 0);

    GreenLedOn = 1;
  }
  //Serial.println(fsrReadingWhite);

  // RED LED
  fsrReadingRed = analogRead(fsrAnalogRed);
  //  Serial.print("Red reading = ");
  //  Serial.println(fsrReadingRed);
  LEDbrightnessRed = map(fsrReadingRed, 0, 1023, 0, 255);

  if (fsrReadingRed > FSRthreshold) {
    analogWrite(ledRed, LEDbrightnessRed);
    tone(buzzer, buzzerRed);
    delay(500);
    noTone(buzzer);
    analogWrite(ledRed, LEDbrightnessRed);
    analogWrite(ledRed, 0);
    delay(500);

    RedLedOn = 1;
  }

  // BLUE LED
  fsrReadingBlue = analogRead(fsrAnalogBlue);
  //      Serial.print("Blue reading = ");
  //    Serial.println(fsrReadingBlue);
  LEDbrightnessBlue = map(fsrReadingBlue, 0, 1023, 0, 255);

  if (fsrReadingBlue > FSRthreshold) {
    analogWrite(ledBlue, LEDbrightnessBlue);
    tone(buzzer, buzzerBlue);
    delay(500);
    noTone(buzzer);
    analogWrite(ledBlue, LEDbrightnessBlue);
    analogWrite(ledBlue, 0);
    BlueLedOn = 1;
  }

  // WHITE LED
  fsrReadingWhite = analogRead(fsrAnalogWhite);
  LEDbrightnessWhite = map(fsrReadingWhite, 0, 1023, 0, 255);

  if (fsrReadingWhite > FSRthreshold) {
    analogWrite(ledWhite, LEDbrightnessWhite);
    tone(buzzer, buzzerWhite);
    delay(500);
    noTone(buzzer);

    analogWrite(ledWhite, 0);
    WhiteLedOn = 1;
  }
  Serial.println(fsrReadingWhite);



}


