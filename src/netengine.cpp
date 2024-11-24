#include "netengine.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

NetEngine::NetEngine(SpatialProcessor& sp) : sp(sp) {}

void NetEngine::begin(const char* ssid, const char* password) {
  this->ssid = ssid;
  this->password = password;

  http = HTTPClient();
  wclient = WiFiClientSecure();
  wclient.setInsecure();

  WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA); //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password); //Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting");

  // Wait for connection
  int no_of_dots = 1;
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(" with IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void NetEngine::update(char* ids) {
  Serial.println("Updating route assigned to this Trekkion device.");
  char url[128];
  snprintf(url, sizeof(url), "https://trekkion.vercel.app/api/device/update?ids=%s", ids);
  
  http.begin(wclient, url);
  
  int httpCode = http.GET();
  String payload = http.getString();

  Serial.print("HTTP GET Response code: ");
  Serial.println(httpCode);
  Serial.print("Payload: ");
  Serial.println(payload);

  sp.parseRoute(payload); // Parse the route
  
  http.end();
}

void NetEngine::post(const char* json, const char* url) {
  Serial.println("Posting ");
  Serial.print("\t");
  Serial.println(json);
  Serial.print("to ");
  Serial.println(url);

  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.begin(wclient, url); 
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(json);
  Serial.print("HTTP POST Response code: ");
  Serial.println(httpCode);



  http.end();
}