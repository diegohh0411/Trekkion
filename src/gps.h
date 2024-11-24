#ifndef TREKK_GPS
#define TREKK_GPS

#include <SoftwareSerial.h>
#include "gps_datum.h"

#define sentence_buffer_size 128
#define no_of_sentences 3

class TrekkGPS {
  private:
    SoftwareSerial gps;
    enum ReadState {
        WAITING,     // Waiting for start marker to come into the SoftwareSerial.
        READING,     // Reading message, taking the start maker as initial character
        READY,       // Have a complete message ready to return
    };
    ReadState state = WAITING;

    char sentences[no_of_sentences][sentence_buffer_size]; // 3 sentences of 128 chars each
    int sdx = 0; // Sentence Index
    int cdx = 0; // Character Index

    void safeCopyString(char* dest, const char* src, size_t destSize);
    void resetSentences();

    double convertNmeaToDecimalDegrees(const char* coordinate, char direction);

    void ParseGPGGA(int dxs, GPSDatum& data); 
    void ParseGPRMC(int dxs, GPSDatum& data);
    void ParseGPGLL(int dxs, GPSDatum& data);
    // GPSDatum GPVTG();
  public:
    TrekkGPS();
    void begin();
    void print();
    void readFromGPS();
    void get(int dxs, GPSDatum& data); // DXS stands for inDeX of Sentence
};

#endif
