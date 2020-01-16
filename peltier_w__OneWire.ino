//included libraries
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  Serial.println("Encapsulant Thermal Cycling Board Temperatures:");
  Serial.println("Order: between A/B, between B/C, between C/D");
  sensors.begin();
  delay(2000);

  sensors.begin();
}

void loop() {

  sensors.requestTemperatures();
  Serial.println();
  Serial.print(sensors.getTempCByIndex(2)); Serial.print(", ");
  Serial.print(sensors.getTempCByIndex(0)); Serial.print(", ");
  Serial.print(sensors.getTempCByIndex(1));

  delay(1000);

}
