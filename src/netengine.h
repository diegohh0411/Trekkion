#ifndef TREKK_NETENGINE
#define TREKK_NETENGINE

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "spatial_logic.h"

class NetEngine {
  private: 
    HTTPClient http;
    WiFiClientSecure wclient;
    SpatialProcessor& sp;

    const char *ssid;  //ENTER YOUR WIFI SETTINGS
    const char *password;  // ENTER YOUR WIFI SETTINGS
  public:
    NetEngine(SpatialProcessor& sp);
    void begin(const char* ssid, const char* password);
    void update(char* ids);
    void post(const char* json, const char* url);
};

#endif