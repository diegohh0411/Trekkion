#include "spatial_logic.h"

#include <ArduinoJson.h>
#include <vector>
#include <cmath>
#include <ESP8266WiFi.h>

SpatialProcessor::SpatialProcessor(int redPin, int greenPin, int bluePin)
  : redPin(redPin), greenPin(greenPin), bluePin(bluePin) {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void SpatialProcessor::parseRoute(const String& json) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  for (JsonObject item : doc.as<JsonArray>()) {
    RoutePoint point;
    point.id_pts = item["id_pts"];
    point.la = item["la"];
    point.lo = item["lo"];
    route.push_back(point);
  }
}

double SpatialProcessor::haversine(double lat1, double lon1, double lat2, double lon2) {
  const double R = 6371.0; // Radius of the Earth in km
  double dLat = (lat2 - lat1) * M_PI / 180.0;
  double dLon = (lon2 - lon1) * M_PI / 180.0;
  lat1 = lat1 * M_PI / 180.0;
  lat2 = lat2 * M_PI / 180.0;

  double a = sin(dLat/2) * sin(dLat/2) +
             sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
  double c = 2 * atan2(sqrt(a), sqrt(1-a));
  return R * c;
}

void SpatialProcessor::checkGPSDatum(double lat, double lon) {
  bool isNear = false;
  for (const auto& point : route) {
    double distance = haversine(lat, lon, point.la, point.lo);
    if (distance <= 0.5) {
      isNear = true;
      break;
    }
  }

  if (isNear) {
    lightUpLED(HIGH); // Green
  } else {
    lightUpLED(LOW); // Red
  }
}

void SpatialProcessor::lightUpLED(int color) {
  if (color == HIGH) {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  } else {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
}