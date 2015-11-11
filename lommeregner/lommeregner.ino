/*
  LiquidCrystal Library - display() and noDisplay()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 display() and noDisplay() functions to turn on and off
 the display.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);

int potPin = 1;
int butPin = 10;
int val1;
int val2;
int operation;
double result;


String sOperator;
String calc;

bool divideBy0 = false;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
 Serial.begin(9600);
  //pinMode(butPin, INPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  /*
  // Turn off the display:
  lcd.noDisplay();
  delay(500);
  // Turn on the display:
  lcd.display();
  delay(500);*/
  divideBy0=false;
  val1 = readPotNum();
  calc = String(val1);
  delay(500);
  operation = getOperator();
  switch(operation){
    case 1:
      sOperator = "+";
      break;
    case 2:
      sOperator = "-";
      break;
    case 3:
      sOperator = "*";
      break;
    case 4:
      sOperator = "/";
      break;
  }
  calc = String(val1)+sOperator;
  delay(500);
  val2 = readPotNum();
  calc = String(val1)+sOperator+String(val2);
  switch(operation){
    case 1:
      result = (double)val1+val2;
      break;
    case 2:
      result = (double)val1-val2;
      break;
    case 3:
      result = (double)val1*val2;
      break;
    case 4:
      if(val2==0){
        divideBy0=true;
      }else{
        result = (double)val1/val2;
      }
      break;
  }
  if(divideBy0){
     lcd.setCursor(0,1);
     lcd.print("ERROR");
     
  }else{
    lcd.setCursor(0,1);
    lcd.print(result);
  }
  delay(500);
  while(!digitalRead(butPin)){}
  calc="";
  delay(500);
}

int readPotNum(){
  int potRead;
  while(!digitalRead(butPin)){
    potRead = analogRead(potPin);
    lcd.clear();
    lcd.print(calc);
    lcd.print(potRead);
    delay(100);
  }
  return potRead;
}
int getOperator(){
  int operation2;
while(!digitalRead(butPin)){
    int potRead = analogRead(potPin);
    operation2;
    lcd.clear();
    lcd.print(calc);
    if(potRead <= 255){
      lcd.print("+");
      operation2=1;
    }else if(potRead<=510){
      lcd.print("-");
      operation2=2;
    }else if(potRead<=765){
      lcd.print("*");
      operation2=3;
    }else{
      lcd.print("/");
      operation2=4;
    }
    delay(100);
    
  }
  return operation2;
}


