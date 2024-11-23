#ifndef TREKK_GPS
#define TREKK_GPS

#include <SoftwareSerial.h>

#define nmea_length 80

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

    public:
        TrekkGPS() : gps(0, 2) {
            gps.begin(9600);
            state = WAITING;
            sentence[0] = '\0';
            index = 0;
        }

        char* read() {
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
                return sentence;
              }
            } else if (current_char == startMarker) {
              state = READING;
              index = 0;
            }
          }

          return nullptr;
        }
};

#endif
