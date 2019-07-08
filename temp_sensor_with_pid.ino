
/***************************************************************************************************************
 * Start python program before beginning cycle
 *
 * When adjusting setpoint: SP listed twice, change preheat function
 ***************************************************************************************************************/

//included libraries
#include <Adafruit_RGBLCDShield.h>
#include <Wire.h>
#include "Adafruit_ADT7410.h"
#include <PID_v1.h>

//temperature sensor
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

//LCD screen
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//universal variables
unsigned long minutes = 60000;
unsigned long startTime = millis();
unsigned long preButton;

//PID and autotune setup
double setpoint = 50, input = 80, output = 50;
double kp = 70, ki = .1, kd = 20;
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);
int windowSize = 5000;
unsigned long windowStartTime;

void setup() {

  delay(1000); //for processing
  
  //LCD and serial port
  lcd.setBacklight(0x5);
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println("Temp. Sensor");
  
  //looking for temperature sensor
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find ADT7410!");
    lcd.print("Sensor not found");
    while (1);
  }

  //SSR controls
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  //pushbutton start
  byte buttons = lcd.readButtons();
  bool start = false;
  while(start == false) {
    lcd.clear();
    Serial.println("Press button to start");
    lcd.print("Press SELECT");
    delay(500);
    if (lcd.readButtons()) {
      if (lcd.readButtons() & BUTTON_SELECT) {
        start = true;
      }
    }
  }
  
 //preparing heating elements for short bursts
 double tempValue = tempsensor.readTempC();
 int currentTemp = tempValue;
 lcd.clear(); lcd.print(tempValue); lcd.print(char(223)); lcd.print("C");
 Serial.println(tempValue);
 if(currentTemp < 40) {
  lcd.setCursor(0,1); lcd.print("Preheating");
  digitalWrite(3, HIGH);
  delay(10000);
  digitalWrite(3, LOW);
  delay(1000);
 }

  //PID and autotune setup
  setpoint = 50;
  myPID.SetOutputLimits(0, windowSize);
  myPID.SetMode(AUTOMATIC);
 

  //timing variables
  unsigned long currentTime = millis();
  preButton = currentTime - startTime;
}

//for lcd shield:
uint8_t i=0;

void loop() {
  
  //delay for temperature sensor
  delay(1000);
  
  //defining variables for future use
  double tempValue = tempsensor.readTempC();
  float timePassed = ((millis() - preButton) / minutes);

  //LCD display/serial monitor: temperature and oven state
  if(digitalRead(3) == HIGH) {
    lcd.clear(); lcd.print(tempValue); lcd.print(char(223)); lcd.print("C"); lcd.print(" (on)");
    Serial.print(tempValue); Serial.println(",1");
  }
  if(digitalRead(3) == LOW) {
     lcd.clear(); lcd.print(tempValue); lcd.print(char(223)); lcd.print("C"); lcd.print(" (off)");
     Serial.print(tempValue); Serial.println(",0");
  }
  
  //LCD display: time passed
  lcd.setCursor(0,1);
  lcd.print("Min passed: "); lcd.print(timePassed, 0);

  //PID stuff
  unsigned long now = (millis() - preButton);
  input = tempValue;
  myPID.Compute();

  //preheat function
  if(tempValue >= 35 && tempValue <= 45) {
    myPID.SetTunings(5,.0,1);
  }
  else{
    myPID.SetTunings(70,.1,20);
  }

  //more PID
  if((now - windowStartTime) > windowSize) { 
      windowStartTime += windowSize;
    }
  if(output > (now - windowStartTime)) digitalWrite(3, HIGH);
    else digitalWrite(3, LOW);
  
  //timer
  if (timePassed >= 30) {
    digitalWrite(3, LOW);
    lcd.clear();
    lcd.print("Cycle complete");
    Serial.print("Cycle complete");
    lcd.setCursor(0,1);
    while(1){
      float newTempValue = tempsensor.readTempC();
      lcd.setCursor(0,1);
      lcd.print("Temp: "); lcd.print(newTempValue); lcd.print((char)223); lcd.print("C");
      Serial.print(newTempValue);
      delay(1000);
    }
  }
}
