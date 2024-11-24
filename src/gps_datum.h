#ifndef GPS_DATUM
#define GPS_DATUM

#include <cstring>
#include <cstdio>

struct GPSDatum {
  char timestamp[12] = {'\0'};

  char latitude[12]  = {'\0'};
  char direction_of_latitude[4] = {'\0'};

  char longitude[12]  = {'\0'};
  char direction_of_longitude[4] = {'\0'};

  char altitude[12]  = {'\0'};
  char unit_of_altitude[2]  = {'\0'};

  char nmea_type[12]  = {'\0'};

  bool isValidFromOrigin = false;

  bool valid() const {
    return timestamp[0] != '\0' &&
           latitude[0] != '\0' &&
           direction_of_latitude[0] != '\0' &&
           longitude[0] != '\0' &&
           direction_of_longitude[0] != '\0' &&
           nmea_type[0] != '\0'
           && isValidFromOrigin;
  }

  void JSON(char* buffer, size_t bufferSize) const {
    snprintf(buffer, bufferSize, 
             "{\"ts\":\"%s\",\"la\":\"%s%s\",\"lo\":\"%s%s\",\"al\":\"%s%s\",\"nt\":\"%s\"}", 
             timestamp, 
             latitude, direction_of_latitude, 
             longitude, direction_of_longitude, 
             altitude, unit_of_altitude, 
             nmea_type);
  }
};

#endif