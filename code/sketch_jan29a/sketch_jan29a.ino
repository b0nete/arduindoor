//
#include <SD.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

// Envs
volatile boolean salida = false;
volatile long int cuenta=0;
volatile long int t0=0;
volatile long int t1=0;
File Registrador1;
DHT dht(DHTPIN, DHTTYPE);
int GLED= 13; // Wet Indicator at Digital PIN D13
int RLED= 12; // Dry Indicator at Digital PIN D12
int SENSE= 0; // Soil Sensor input at Analog PIN A0
int value= 0;

// Setup
void setup() {
  Serial.begin(9600);
    while (!Serial) {
    ; // Espera a que el puerto serial se inicialize, solo es necesario para el Leonardo. 
  }
  pinMode(10, OUTPUT);  //Comentar para el MEGA, descomentar para el UNO

  if (!SD.begin(4)) {   //El 4 se refiere al pin del CS. Para este shield es el pin 4.
    Serial.println("No se puede inicializar la micro-SD");
    return;
  }
  Serial.println("micro-SD inicializada");

  //
  Serial.println(F("DHTxx test!"));
  dht.begin();
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  Serial.println("SOIL MOISTURE SENSOR");
  Serial.println("-----------------------------");


}

// Functions 
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


void registrarDato(unsigned int dato){
    Registrador1 = SD.open("LOG.TXT", FILE_WRITE);  //Abre y/o crea el archivo LOG.TXT
  
  // Si se abre el archivo escribir datos. 
  if (Registrador1) {
      Registrador1.print(dato);
      Registrador1.print(";");
      Registrador1.println((long int)millis());
      Registrador1.close();
      Serial.print("Se han guardado: ");
      Serial.print(cuenta++);
      Serial.println(" datos");
  }
  else{
    Serial.println("Error no se puede abrir el archivo LOG.TXT");
  }
}

void serialEvent() {
  if (Serial.available() > 0) {
    //Leer un nuevo Byte serial
    char datoSerialByte = (char)Serial.read();
    if(datoSerialByte == 'A') {
        salida = true;
        Serial.println("Es seguro retirar la memoria");
    }
    else if(datoSerialByte == 'B'){
        salida = false;
    }
  }
}


// Init
void loop() {
  delay(5000);
  readHumidityGroundSensor();
  readTemperatureAndHumidity();

  if(salida == false) {
          registrarDato(analogRead(A0));
          t1=millis()+2000;
        }
}
