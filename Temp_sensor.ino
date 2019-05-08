
//Currently, code...
    //Records temperature of ADT7410 and sends to serial port (so you can see past values)
    //Reads temperature of ADT7410 on first line of 16x2 lcd
    //Reads number of minutes passed since last reset on second line of 16x2 lcd
    //Stops running after 30 minutes (tested, simply reset board, didn't stop)

//Wiring:
    //ADT7410: 5V to VIN, GND to GND, SCL to SCL, SDA to SDA
    //16x2 lcd, by pin number (see Arduino pin values in code):
      //1: GND
      //2: 5V
      //3: center of pot
      //4: rs
      //5: GND
      //6: en
      //7-10: ---
      //11: d4
      //12: d5
      //13: d6
      //14: d7
      //15: 5V
      //16: GND
      //17-18: ---

//Plan:
  //Add turning on via pushbutton
  //Look into solid state relay, then go from there

#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_ADT7410.h"
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410(); //names temperature sensor
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
  //lets me refer to Arduino pins by what lcd pin is connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  //defines what pins connected to lcd
unsigned long minutes = 60000; //lets me calculate minutes later; number too long for normal variable

void setup() {
  lcd.begin(16, 2); //sets up number of rows/columns
  Serial.begin(115200); //match with serial monitor; not sure (yet) which value best to use
  Serial.println("Temp. Sensor"); //initially sends to serial port
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find ADT7410!");
    while (1);
  } //don't exactly understand statement; copied from sensor test code to check for sensor
  delay(250); //sensor takes .25 seconds for first reading
}

void loop() {
  float tempValue = tempsensor.readTempC(); //defines variable of temperature
  Serial.print("Temp: "); Serial.print (tempValue); Serial.println (" *C"); //print reading to serial port
  lcd.clear(); //clears lcd screen
  lcd.write("Temp: "); lcd.print(tempValue); lcd.write(" *C"); //prints temp to lcd
  lcd.setCursor(0,1); //moves cursor to second row
  float timePassed = millis() / minutes; //defines variable of minutes passed
  lcd.write("Min passed: "); lcd.print(timePassed, 0); //prints minutes passed to lcd
  if (timePassed > 30 ) { //if running for longer than 30 minutes
    while(1); //will never be false, loops forever after 30 minutes
    //didn't work after testing - reset "min passed"
    }
  delay(1000); //delays reading for temperature sensor
}
