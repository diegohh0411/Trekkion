#include "gps.h"

#include <cstring>
#include <iostream>

void TrekkGPS::safeCopyString(char* dest, const char* src, size_t destSize) {
    size_t i;
    
    // Copy characters one by one up to destSize-1
    for(i = 0; i < destSize - 1 && src[i] != '\0'; i++)
      dest[i] = src[i];
    
    // Always null terminate
    dest[i] = '\0';
}

void TrekkGPS::resetSentences() {
  for (int i = 0; i < no_of_sentences; i++) {
    for (int u = 0; u < sentence_buffer_size; u++) {
      sentences[i][u] = '\0';
    }
  }
}

TrekkGPS::TrekkGPS() : gps(0, 2) {}

void TrekkGPS::begin() {
  gps.begin(9600);
  resetSentences();
}

void TrekkGPS::print() {
  Serial.println("[TrekkGPS] Current state:");
  for (int i = 0; i < no_of_sentences; i++) {
    Serial.print("[");
    Serial.print(i);
    Serial.print("] ");
    
    Serial.println(sentences[i]);
  }
}

void TrekkGPS::readFromGPS() {
  char startMarker = '$';
  char endMarker = '\n';
  char currentChar;

  if (state == READY) {
    resetSentences();
    state = WAITING;
  }

  while (gps.available() > 0 && state != READY) {
    currentChar = gps.read();

    if (currentChar == startMarker && state != READING) {
      state = READING;
      sdx = 0;
      cdx = 0;
    } else if (state == READING) {
      if (currentChar == endMarker) {
        sentences[sdx][cdx] = '\0';
        state = READY;
      } else if (currentChar == startMarker) {
        sentences[sdx][cdx] = '\0';
        sdx++;
        cdx = 0; // Reset to start of sentence.

        if (sdx >= no_of_sentences)
          sdx = no_of_sentences - 1;
      } else {
        sentences[sdx][cdx] = currentChar;
        cdx++;
        
        if (cdx >= sentence_buffer_size)
          cdx = sentence_buffer_size - 1;
      }
    }
  }
}

void TrekkGPS::ParseGPGGA(int dxs, GPSDatum& data) { // DXS stands for inDeX of Sentence
  if (strncmp(sentences[dxs], "GPGGA", 5) == 0) {
    safeCopyString(data.nmea_type, "GPGGA", sizeof(data.nmea_type));

    // Make a copy of the sentence to avoid modifying original
    char tempSentence[sentence_buffer_size];
    safeCopyString(tempSentence, sentences[dxs], sizeof(tempSentence));

    char* token = strtok(tempSentence, ",");
    int fieldIndex = 0;

    while (token) {
      switch(fieldIndex) {
        case 1: // UTC Time
          safeCopyString(data.timestamp, token, sizeof(data.timestamp)); break;
        case 2: // Latitude
          safeCopyString(data.latitude, token, sizeof(data.latitude)); break;
        case 3: // N/S Indicator
          safeCopyString(data.direction_of_latitude, token, sizeof(data.direction_of_latitude)); break;
        case 4: // Longitude
          safeCopyString(data.longitude, token, sizeof(data.longitude)); break;
        case 5: // E/W Indicator
          safeCopyString(data.direction_of_longitude, token, sizeof(data.direction_of_longitude)); break;
        case 6: // GPS Quality Indicator: 0 = Fix not valid, 1 = GPS fix, 2 = DGPS fix, 4 = RTK fixed, 5 = RTK float
          if (token[0] == '0')
            return;
          break;
        case 9: // Altitude (above ellipsoid) in meters
          safeCopyString(data.altitude, token, sizeof(data.altitude)); break;
        case 10: // Altitude unit (M for meters)
          safeCopyString(data.unit_of_altitude, token, sizeof(data.unit_of_altitude)); break;
      }
        
      token = strtok(nullptr, ",");
      fieldIndex++;
    }

    data.isValidFromOrigin = true;
  }
}

void TrekkGPS::ParseGPRMC(int dxs, GPSDatum& data) { // DXS stands for inDeX of Sentence
  if (strncmp(sentences[dxs], "GPRMC", 5) == 0) {
    safeCopyString(data.nmea_type, "GPRMC", sizeof(data.nmea_type));

    // Make a copy of the sentence to avoid modifying original
    char tempSentence[sentence_buffer_size];
    safeCopyString(tempSentence, sentences[dxs], sizeof(tempSentence));

    char* token = strtok(tempSentence, ",");
    int fieldIndex = 0;

    while (token) {
      switch(fieldIndex) {
        case 1: // UTC Time
          safeCopyString(data.timestamp, token, sizeof(data.timestamp)); break;
        case 2: // Data status: "A" for valid data, "V" for warning
          if (token[0] != 'A')
            return;
        case 3: // Latitude
          safeCopyString(data.latitude, token, sizeof(data.latitude)); break;
        case 4: // N/S Indicator
          safeCopyString(data.direction_of_latitude, token, sizeof(data.direction_of_latitude)); break;
        case 5: // Longitude
          safeCopyString(data.longitude, token, sizeof(data.longitude)); break;
        case 6: // E/W Indicator
          safeCopyString(data.direction_of_longitude, token, sizeof(data.direction_of_longitude)); break;
      }
        
      token = strtok(nullptr, ",");
      fieldIndex++;
    }

    data.isValidFromOrigin = true;
  }
}

void TrekkGPS::ParseGPGLL(int dxs, GPSDatum& data) { // DXS stands for inDeX of Sentence
  if (strncmp(sentences[dxs], "GPGLL", 5) == 0) {
    safeCopyString(data.nmea_type, "GPGLL", sizeof(data.nmea_type));

    // Make a copy of the sentence to avoid modifying original
    char tempSentence[sentence_buffer_size];
    safeCopyString(tempSentence, sentences[dxs], sizeof(tempSentence));

    char* token = strtok(tempSentence, ",");
    int fieldIndex = 0;

    while (token) {
      switch(fieldIndex) {
        case 1: // Latitude
          safeCopyString(data.latitude, token, sizeof(data.latitude)); break;
        case 2: // N/S Indicator
          safeCopyString(data.direction_of_latitude, token, sizeof(data.direction_of_latitude)); break;
        case 3: // Longitude
          safeCopyString(data.longitude, token, sizeof(data.longitude)); break;
        case 4: // E/W Indicator
          safeCopyString(data.direction_of_longitude, token, sizeof(data.direction_of_longitude)); break;
        case 5: // UTC Time
          safeCopyString(data.timestamp, token, sizeof(data.timestamp)); break;
        case 6: // Data status: "A" for valid data, "V" for warning
          if (token[0] != 'A')
            return;
      }
        
      token = strtok(nullptr, ",");
      fieldIndex++;
    }

    data.isValidFromOrigin = true;
  }
}

void TrekkGPS::get(int dxs, GPSDatum& data) { // DXS stands for inDeX of Sentence
  data.isValidFromOrigin = false;

  if (dxs >= 0 && dxs < no_of_sentences) {
    ParseGPGGA(dxs, data);

    if (!data.valid())
      ParseGPRMC(dxs, data);

    if (!data.valid())
      ParseGPGLL(dxs, data);

  }
}