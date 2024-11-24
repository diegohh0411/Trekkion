#include "trekk_dht.h"
#include "device_spec.h"

TrekkDHT::TrekkDHT() : dht(DHT_PIN, DHTTYPE) {}

void TrekkDHT::setTimestamp(char* ts) {
  strncpy(timestamp, ts, sizeof(timestamp));
}

void TrekkDHT::JSON(char* buffer, size_t bufferSize) {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  snprintf(buffer, bufferSize, "{\"ts\":\"%s\",\"h\":\"%f\",\"t\":\"%f\",\"ids\":\"%s\", \"idr\":\"%s\"}", timestamp, h, t, ids, idr);
}