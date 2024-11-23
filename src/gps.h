#ifndef TREKK_GPS
#define TREKK_GPS

#include <SoftwareSerial.h>
#include <cstring>
#include <iostream>

#define nmea_length 80

struct GPSData {
  char timestamp[12] = {};

  char latitude[12]  = {};
  char direction_of_latitude[4] = {};

  char longitude[12]  = {};
  char direction_of_longitude[4] = {};

  char altitude[12]  = {};
  char unit_of_altitude[2]  = {};

  char nmea_type[12]  = {};

  bool valid = false;

  void print() const {
    Serial.print("GPSDatum { ");
    Serial.print("ts: ");
    Serial.print(timestamp);
    Serial.print(", la: ");
    Serial.print(latitude);
    Serial.print(direction_of_latitude);
    Serial.print(", lo: ");
    Serial.print(longitude);
    Serial.print(direction_of_longitude);
    Serial.print(", al: ");
    Serial.print(altitude);
    Serial.print(unit_of_altitude);
    Serial.print(", nt: ");
    Serial.print(nmea_type);
    Serial.println("}");
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

        void safeCopyString(char* dest, const char* src, size_t destSize);
        void resetSentence();
        GPSData get();

        GPSData GPGGA(); 
        // GPSData GPRMC(); // https://www.perplexity.ai/search/what-is-a-struct-in-cpp-GA3lTLaESmuY07l6ZytGLg#10
        // GPSData GPGLL();
        // GPSData GPVTG();
    public:
        TrekkGPS() : gps(0, 2) {
            gps.begin(9600);
            state = WAITING;
            sentence[0] = '\0';
            index = 0;
        }

        GPSData read();
        void printCurrentSentence();
};

void TrekkGPS::safeCopyString(char* dest, const char* src, size_t destSize) {
    size_t i;
    
    // Copy characters one by one up to destSize-1
    for(i = 0; i < destSize - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    
    // Always null terminate
    dest[i] = '\0';
}

void TrekkGPS::resetSentence() {
  memset(sentence, 0, sizeof(sentence));
}

GPSData TrekkGPS::get() {
  char startMarker = '$';
  char endMarker = '\n';
  char currentChar;

  if (state == READY) {
    resetSentence();
    state = WAITING;
  }

  while (gps.available() > 0 && state != READY) {
    currentChar = gps.read();

    if (state == READING) {
      if (currentChar != endMarker) {
        sentence[index] = currentChar;
        index++;
        if (index >= nmea_length) {
          index = nmea_length - 1;
        }
      } else {
        sentence[index] = '\0';
        state = READY;
      }
    } else if (currentChar == startMarker) {
      state = READING;
      index = 0;
    }
  }

  return GPGGA();
}

GPSData TrekkGPS::GPGGA() {
  Serial.print("Current sent. at start of `GPGGA` sentence is: ");
  Serial.println(sentence);

  GPSData d;

  if (strncmp(sentence, "GPGGA", 5) == 0) {
    safeCopyString(d.nmea_type, "GPGGA", sizeof(d.nmea_type));

    char* token = strtok(sentence, ",");
    int fieldIndex = 0;

    while (token) {
      switch(fieldIndex) {
        case 1: // UTC Time
          safeCopyString(d.timestamp, token, sizeof(d.timestamp)); break;
        case 2: // Latitude
          safeCopyString(d.latitude, token, sizeof(d.latitude)); break;
        case 3: // N/S Indicator
          safeCopyString(d.direction_of_latitude, token, sizeof(d.direction_of_latitude)); break;
        case 4: // Longitude
          safeCopyString(d.longitude, token, sizeof(d.longitude)); break;
        case 5: // E/W Indicator
          safeCopyString(d.direction_of_longitude, token, sizeof(d.direction_of_longitude)); break;
        case 6: // GPS Quality Indicator: 0 = Fix not valid, 1 = GPS fix, 2 = DGPS fix, 4 = RTK fixed, 5 = RTK float
          if (token[0] == '0')
            return d; // Return GPSData while it is not valid.
          break;
        case 9: // Altitude (above ellipsoid) in meters
          safeCopyString(d.altitude, token, sizeof(d.altitude)); break;
        case 10: // Altitude unit (M for meters)
          safeCopyString(d.unit_of_altitude, token, sizeof(d.unit_of_altitude)); break;
      }
      
      token = strtok(nullptr, ",");
      fieldIndex++;
    }

    d.valid = true;
  }

  return d;
}

void TrekkGPS::printCurrentSentence() {
  Serial.println(sentence);
}

#endif
