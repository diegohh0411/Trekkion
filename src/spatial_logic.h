#ifndef TREKK_SPATIAL_LOGIC
#define TREKK_SPATIAL_LOGIC

#include <ArduinoJson.h>
#include <vector>
#include <cmath>
#include <ESP8266WiFi.h>

struct RoutePoint {
  int id_pts;
  double la;
  double lo;
};

class SpatialProcessor {
  public:
    SpatialProcessor(int redPin, int greenPin, int bluePin);
    void parseRoute(const String& json);
    void checkGPSDatum(double lat, double lon);

  private:
    std::vector<RoutePoint> route;
    int redPin, greenPin, bluePin;

    double haversine(double lat1, double lon1, double lat2, double lon2);
    void lightUpLED(int color);
};

#endif