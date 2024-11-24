#include "gps.h"
#include "trekk_dht.h"
#include <SoftwareSerial.h>
#include "netengine.h"
#include "spatial_logic.h"

#include "device_spec.h"
char ids[12] = { '1', '\0' };
char idr[12] = { '1', '\0' };

TrekkGPS gps;
TrekkDHT tdht;

SpatialProcessor sp(12, 13, 14);
NetEngine neteng(sp);

void setup() {
  Serial.begin(9600);
  Serial.println("Trekkion setup begins.");

  gps.begin();
  neteng.begin("regular II", "librosycafe");

  neteng.update(ids);

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
      neteng.post(json, "https://trekkion.vercel.app/api/gpsdatum");

      sp.checkGPSDatum(currentData.decimal_latitude, currentData.decimal_longitude);

      // tdht.setTimestamp(currentData.timestamp);
      // tdht.JSON(json, sizeof(json));
    }
  }

  Serial.println();
  gps.print();

  delay(1000);
}