
/*
Currently, code...
    Records temperature sends to serial port/plotter (so you can see past values)
    Reads temperature on first line of lcd
    Reads number of minutes passed since button pressed on second line of lcd
    Stops running after a value, cuts power to oven, continues displaying temperature
    Turns on oven if below a value, off if above a value
    Waits to begin until button pressed

Wiring:
    Sensor: 5V to VIN, GND to GND, SCL to SCL, SDA to SDA
    LCD:
      1: GND
      2: 5V
      3: center of pot
      4: rs (12)
      5: GND
      6: en (11)
      7-10: ---
      11: d4
      12: d5
      13: d6
      14: d7
      15: 5V
      16: GND
      17-18: ---
    Pushbutton: pin 9, opposite corner GND
    SSR: pin 8, other side GND

Current values:
      Runs for 60 minutes
      On if below/equal to 59 C
      Off if above 61 C

Plan:
  Look into restart due to serial port opening
  Test and see if PID wanted/needed
*/

//NOTE: Opening serial port/plotter will restart cycle; open before pressing button

#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_ADT7410.h"

Adafruit_ADT7410 tempsensor = Adafruit_ADT7410(); //names temperature sensor
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
  //lets me refer to Arduino pins by what lcd pin is connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  //defines what pins connected to lcd
unsigned long minutes = 60000; //lets me calculate minutes later; number too long for normal variable
unsigned long startTime = millis();
unsigned long preButton;

void setup() {
  lcd.begin(16, 2); //sets up number of rows/columns
  Serial.begin(115200); //match with serial monitor; not sure (yet) which value best to use
  Serial.println("Temp. Sensor"); //initially sends to serial port
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find ADT7410!");
    while (1);
  } //doesn't continue unless sensor found
  pinMode(8, OUTPUT); //output pin for SSR
  digitalWrite(8, HIGH); //stops power from flowing to oven until turned on
  const byte buttonPin = 9; //naming and setting a constant 0-255
  pinMode(buttonPin, INPUT_PULLUP); //input pin for pushbutton
  while(digitalRead(9) == HIGH) {
    Serial.println("Waiting");
    lcd.write("Waiting...");
    delay(500);
    lcd.clear();
  }
  unsigned long currentTime = millis();
  preButton = currentTime - startTime; //time passed before button pressed
}

void loop() {
  delay(1000); //delays reading for temperature sensor
  float tempValue = tempsensor.readTempC(); //defines variable of temperature
  Serial.print("Temp: "); Serial.print(tempValue); Serial.println(" *C"); //print reading to serial port
  lcd.clear(); //clears lcd screen
  lcd.write("Temp: "); lcd.print(tempValue); lcd.write(" *C"); //prints temp to lcd
  lcd.setCursor(0,1); //moves cursor to second row
  float timePassed = ((millis() - preButton) / minutes); //defines variable of minutes passed
  lcd.write("Min passed: "); lcd.print(timePassed, 0); //prints minutes passed to lcd
  if (tempValue <= 59) { //if cooler than 60 degrees celcius
    digitalWrite(8, LOW); //allow power to oven
  }
  if (tempValue > 61) { //if warmer than 61 degrees celcius
    digitalWrite(8, HIGH); //cut power to oven
  }
  if (timePassed > 3) { //if running for longer than one hour
    digitalWrite(8, HIGH); //cuts power
    lcd.clear();
    lcd.write("Cycle complete"); //displays message
    lcd.setCursor(0,1);
    while(1){
      float newTempValue = tempsensor.readTempC();
      lcd.setCursor(0,1);
      lcd.write("Temp: "); lcd.print(newTempValue); lcd.write(" *C"); //continues to update temp
      Serial.print("Temp: "); Serial.print(newTempValue); Serial.println(" *C");
      delay(1000); //delays reading for temperature sensor
    }
  }
}
