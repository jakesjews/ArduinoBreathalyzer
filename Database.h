#include <Arduino.h>

struct HighScore {
  char initials[4];
  uint8_t bac;
};

void setupDatabase();
void writeScore(HighScore score);
void printScores();
void clearDatabase();
