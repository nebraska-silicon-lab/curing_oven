
/***************************************************************************************************************
 * LCD: (1-GND), (2-5V), (3-POT), (4-rs), (5-GND), (6-en), (11-d4), (12-d5), (13-d6), (14-d7), (15-5V), (16-GND)
 * PUSHBUTTON: 9, GND
 * RELAY: 3, GND
 * SENSOR: labeled
 * 
 * Open serial monitor before beginning cycle
 ***************************************************************************************************************/

//Included libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_ADT7410.h"
#include <PID_v1.h>

//Temperature sensor
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410(); //names temperature sensor

//LCD screen
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Universal variables
unsigned long minutes = 60000;
unsigned long startTime = millis();
unsigned long preButton;

//PID setup
double Setpoint, Input, Output;
double Kp = 3, Ki = 5, Kd = 1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int windowSize = 5000; //length of PWM cycle
unsigned long windowStartTime;

void setup() {
  //LCD and serial port
  lcd.begin(16, 2); //sets up number of rows/columns
  Serial.begin(115200); //match with serial monitor; not sure (yet) which value best to use
  Serial.println("Temp. Sensor"); //initially sends to serial port
  
  //Looking for temperature sensor
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find ADT7410!");
    while (1);
  }

  //SSR controls
  pinMode(3, OUTPUT); //output pin for SSR
  digitalWrite(3, LOW); //stops power from flowing to oven until turned on

  //Pushbutton start
  const byte buttonPin = 9;
  pinMode(buttonPin, INPUT_PULLUP); //input pin for pushbutton
  while(digitalRead(9) == HIGH) {
    Serial.println("Waiting");
    lcd.write("Waiting...");
    delay(500);
    lcd.clear();
  }

  //PID setup
  Setpoint = 50;
  myPID.SetOutputLimits(0, windowSize);
  myPID.SetMode(AUTOMATIC);

  //Timing variables
  unsigned long currentTime = millis();
  preButton = currentTime - startTime; //time passed before button pressed
}

void loop() {
  //delay for temperature sensor
  delay(1000);
  
  //defining variables for future use
  double tempValue = tempsensor.readTempC();
  float timePassed = ((millis() - preButton) / minutes);

  //LCD display/serial monitor: temperature and oven state
  if(digitalRead(3) == HIGH) {
    lcd.clear(); lcd.write("Oven on  - "); lcd.print(tempValue);
  }
  if(digitalRead(3) == LOW) {
    lcd.clear(); lcd.write("Oven off - "); lcd.print(tempValue);
  }
  Serial.println(tempValue);
  
  //LCD display: time passed
  lcd.setCursor(0,1);
  lcd.write("Min passed: "); lcd.print(timePassed, 0);
  
  //PID functions
  Input = tempValue;
  myPID.Compute();
  unsigned long now = (millis() - preButton);
  if ((now - windowStartTime) > windowSize) {
    windowStartTime += windowSize;
  }
  if (Output > (now - windowStartTime)) digitalWrite(3, HIGH); //turn on PID
  else digitalWrite(3, LOW); //turn off PID

  //Timer
  if (timePassed >= 20) {
    digitalWrite(3, LOW);
    lcd.clear();
    lcd.write("Cycle complete");
    lcd.setCursor(0,1);
    while(1){
      float newTempValue = tempsensor.readTempC();
      lcd.setCursor(0,1);
      lcd.write("Temp: "); lcd.print(newTempValue); lcd.write(" *C");
      Serial.print("Temp: "); Serial.print(newTempValue); Serial.println(" *C");
      delay(1000);
    } //if timer expires, cut power to oven, display temp values
  }
}
