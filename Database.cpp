#include "Database.h"
#include <EEPROM.h>

int16_t currentIndex = 0;

int16_t getBlankIndex() {
  for (int16_t i = 0; i < EEPROM.length(); i += sizeof(HighScore)) {
    if (EEPROM[i] == 0) {
      return i;
    }
  }
  return 0;
}

void setupDatabase() {
  currentIndex = getBlankIndex();
}

void writeScore(HighScore score) {
  EEPROM.put(currentIndex, score);
  currentIndex += sizeof(HighScore);
}

void printScores() {
  for (int16_t i = 0; i < currentIndex; i += sizeof(HighScore)) {
    HighScore toPrint;
    EEPROM.get(i, toPrint);
    Serial.print(toPrint.initials);
    Serial.print(": ");
    Serial.println(toPrint.bac);
  }
}

