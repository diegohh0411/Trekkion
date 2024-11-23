#include "gps.h"
#include <SoftwareSerial.h>

TrekkGPS gps;

void setup() {
  Serial.begin(9600);
  Serial.println("Trekkion setup began.");
}

void loop() {
  Serial.println(gps.read());

  delay(1000);
}