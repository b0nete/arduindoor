#include <SD.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int GLED= 13; // Wet Indicator at Digital PIN D13
int RLED= 12; // Dry Indicator at Digital PIN D12
int SENSE= 0; // Soil Sensor input at Analog PIN A0
int value= 0;
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  Serial.println("SOIL MOISTURE SENSOR");
  Serial.println("-----------------------------");
}
void readTemperatureAndHumidity() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
void readHumidityGroundSensor() {
  value= analogRead(SENSE);
   value= value/10;
   Serial.print("Sensor de humedad tierra : ");
   Serial.println(value);
   if(value<50)
   {
      digitalWrite(GLED, HIGH);
   }
   else
   {
      digitalWrite(RLED,HIGH);
   }
   delay(1000);
   digitalWrite(GLED,LOW);
   digitalWrite(RLED, LOW);
}
void loop() {
  delay(2000);
  readHumidityGroundSensor();
  readTemperatureAndHumidity();
}
