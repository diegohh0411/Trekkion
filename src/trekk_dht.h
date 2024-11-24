#ifndef TREKK_DHT
#define TREKK_DHT

#include <DHT.h>

#define DHT_PIN 5
#define DHTTYPE DHT11

class TrekkDHT {
private:
  char timestamp[12] = {'\0'};
  DHT dht;
public:
  TrekkDHT();

  void setTimestamp(char* ts);

  void JSON(char* buffer, size_t bufferSize);
};

#endif