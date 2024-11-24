#include "gps.h"
#include <SoftwareSerial.h>

TrekkGPS gps;

void setup() {
  Serial.begin(9600);
  Serial.println("Trekkion setup begins.");

  gps.begin();

  Serial.println("Trekkion setups ends.");
}

GPSDatum currentData;
#define json_buffer_size 128
char json[json_buffer_size];

void loop() {
  gps.readFromGPS();
  
  for (int i = 0; i < no_of_sentences; i++) {
    gps.get(i, currentData);

    if (currentData.valid()) {
      currentData.JSON(json, sizeof(json));
      Serial.println(json);
    }
  }

  delay(1000);
}