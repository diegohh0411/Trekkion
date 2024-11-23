#include "gps.h"
#include <SoftwareSerial.h>

TrekkGPS gps;
char* nmea_sentence;

void setup() {
  Serial.begin(9600);
  Serial.println("Trekkion setup began.");
}

void loop() {
  GPSDatum d = gps.read();

  if (d.valid)
    d.print();

  delay(1000);
}