#ifndef GPS_DATUM
#define GPS_DATUM

#include <cstring>
#include <cstdio>
#include "device_spec.h"

struct GPSDatum {
  char timestamp[12] = {'\0'};

  char latitude[12]  = {'\0'};
  char direction_of_latitude[4] = {'\0'};
  double decimal_latitude;

  char longitude[12]  = {'\0'};
  char direction_of_longitude[4] = {'\0'};
  double decimal_longitude;

  char altitude[12]  = {'\0'};
  char unit_of_altitude[2]  = {'\0'};

  char nmea_type[12]  = {'\0'};

  bool isValidFromOrigin = false;

  char originalNmea[128];

  bool valid() const {
    return timestamp[0] != '\0' &&
           latitude[0] != '\0' &&
           decimal_latitude != 0.0 &&
           direction_of_latitude[0] != '\0' &&
           longitude[0] != '\0' &&
           decimal_longitude != 0.0 &&
           direction_of_longitude[0] != '\0' &&
           nmea_type[0] != '\0' && 
           isValidFromOrigin;
  }

  void JSON(char* buffer, size_t bufferSize) const {
    snprintf(buffer, bufferSize, 
             "{\"ts\":\"%s\",\"la\":\"%f\",\"lo\":\"%f\",\"al\":\"%s%s\",\"nt\":\"%s\",\"ids\":\"%s\", \"idr\":\"%s\"}", 
             timestamp, 
             decimal_latitude,
             decimal_longitude,
             altitude, unit_of_altitude, 
             nmea_type,
             ids,
             idr
          );
  }
};

#endif