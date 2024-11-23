#include "gps.h"
#include <SoftwareSerial.h>

TrekkGPS gps;

void setup() {
  Serial.begin(9600);
  Serial.println("Trekkion setup begins.");
  Serial.print("HE");
  gps.begin();
  Serial.println("Y");
  Serial.println("Trekkion setups ends.");
}

GPSData currentData;
void loop() {
  gps.readFromGPS();
  gps.print();
  
  for (int i = 0; i < no_of_sentences; i++) {
    gps.get(i, currentData);

    if (currentData.valid) {
      currentData.JSON();
    }
  }

  delay(5000);
}