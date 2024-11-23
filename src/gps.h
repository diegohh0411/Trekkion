#ifndef TREKK_GPS
#define TREKK_GPS

#include <SoftwareSerial.h>

#define sentence_buffer 128
#define no_of_sentences 3

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

  void JSON() const {
    Serial.print("{");
    Serial.print("\"ts\":\"");
    Serial.print(timestamp);
    Serial.print("\",\"la:\"");
    Serial.print(latitude);
    Serial.print(direction_of_latitude);
    Serial.print("\",\"lo:\"");
    Serial.print(longitude);
    Serial.print(direction_of_longitude);
    Serial.print("\",\"al:\"");
    Serial.print(altitude);
    Serial.print(unit_of_altitude);
    Serial.print("\",\"nt:\"");
    Serial.print(nmea_type);
    Serial.println("\"}");
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
    ReadState state = WAITING;

    char sentences[no_of_sentences][sentence_buffer]; // 3 sentences of 128 chars each
    int sdx = 0; // Sentence Index
    int cdx = 0; // Character Index

    void safeCopyString(char* dest, const char* src, size_t destSize);
    void resetSentences();
    void ParseGPGGA(int dxs, GPSData& data); 
    void ParseGPRMC(int dxs, GPSData& data); // https://www.perplexity.ai/search/what-is-a-struct-in-cpp-GA3lTLaESmuY07l6ZytGLg#10
    // GPSData GPGLL();
    // GPSData GPVTG();
  public:
    TrekkGPS();
    void begin();
    void print();
    void readFromGPS();
    void get(int dxs, GPSData& data); // DXS stands for inDeX of Sentence
};

#endif
