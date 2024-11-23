#ifndef TREKK_GPS
#define TREKK_GPS

#include <SoftwareSerial.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define nmea_length 80

struct GPSDatum {
  char* timestamp;
  float lat;
  float lng;
  bool valid = false;

  void print() const {
    Serial.print("Timestamp: ");
    Serial.print(timestamp ? timestamp : "N/A");
    Serial.print(", Latitude: ");
    Serial.print(lat, 6); // Print with 6 decimal places
    Serial.print(", Longitude: ");
    Serial.println(lng, 6); // Print with 6 decimal places
  }
};

class TrekkGPS {
    private:
        SoftwareSerial gps;
        enum ReadState {
            WAITING,     // Waiting for start marker to come into the SoftwareSerial.
            READING,     // Reading message, taking the start maker as initial character
            READY,       // Have a complete message ready to return
        };
        ReadState state;
        char sentence[128];
        byte index;

        void updateSentece();
        GPSDatum GPGGA(); 
        GPSDatum GPRMC(); // https://www.perplexity.ai/search/what-is-a-struct-in-cpp-GA3lTLaESmuY07l6ZytGLg#10
        GPSDatum GPGLL();
        GPSDatum GPVTG();
    public:
        TrekkGPS() : gps(0, 2) {
            gps.begin(9600);
            state = WAITING;
            sentence[0] = '\0';
            index = 0;
        }

        GPSDatum read();
};

void TrekkGPS::updateSentece() {
  char startMarker = '$';
  char endMarker = '\n';
  char current_char;

  if (state == READY) {
    state = WAITING;
  }

  while (gps.available() > 0 && state != READY) {
    current_char = gps.read();

    if (state == READING) {
      if (current_char != endMarker) {
        sentence[index] = current_char;
        index++;
        if (index >= nmea_length) {
          index = nmea_length - 1;
        }
      } else {
        sentence[index] = '\0';
        state = WAITING;
      }
    } else if (current_char == startMarker) {
      state = READING;
      index = 0;
    }
  }
}

GPSDatum TrekkGPS::GPGGA() {
  GPSDatum d;

  if (strncmp(sentence, "GPGGA", 5) == 0) {
    char* token = strtok(sentence, ",");
    int fieldIndex = 0;

    while (token) {
      switch(fieldIndex) {
        case 1: // UTC Time
          d.timestamp = token;
          break;
        case 2: // Latitude
          d.lat = atof(token);
          break;
        case 3: // N/S Indicator
          if (token[0] == 'S')
            d.lat = -d.lat; // Convert to negative if in Southern Hemisphere
          break;
        case 4: // Longitude
          d.lng = atof(token);
          break;
        case 5:
          if (token[0] == 'W')
            d.lng = -d.lng; // Convert to negative if in Southern Hemisphere
          break;
      }
      
      token = strtok(nullptr, ",");
      fieldIndex++;
    }

    d.valid = true;
  }

  return d;
}

GPSDatum TrekkGPS::read() {
  updateSentece();

  GPSDatum d;

  d = GPGGA();
  if (d.valid) return d;

  return d;
}

#endif
