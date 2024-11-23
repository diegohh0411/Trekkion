#include "gps.h"
#include <SoftwareSerial.h>

TrekkGPS gps;

void setup() {
  Serial.begin(9600);
  Serial.println("Trekkion setup begins.");

  Serial.println("Trekkion setups ends.");
}

void loop() {
  GPSData d = gps.read();
  if (d.valid) {
    d.print();
    gps.printCurrentSentence();
  }

  // delay(1000);
}