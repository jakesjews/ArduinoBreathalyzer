#include <Thread.h>
#include <Wire.h>
#include "FourDigitLCD.h"
#include "BacLookup.h"
#include "Database.h"
#include "Bacformat.h"

#define DEBUG       0
#define SENSOR_PIN  A5
#define S7_ADDR     0x71
#define DEC_MASK    0b00000001
#define BAC_START   410     // Beginning ADC value of BAC chart
#define BAC_END     859     // Last ADC value in BAC chart

int sensorRead;
uint8_t bac;
FourDigitLCD lcd;
String displayedBac = String("000");
Thread bacThread = Thread();
Thread displayThread = Thread();
Thread storageThread = Thread();

void setup() {
  #if DEBUG
  Serial.begin(9600);
  while (!Serial) { ; }
  #endif
  
  lcd.init();
  pinMode(SENSOR_PIN, INPUT);
  
  bacThread.onRun(setBac);
  bacThread.setInterval(100);

  displayThread.onRun(displayBac);
  displayThread.setInterval(1);

  storageThread.onRun(recordScore);
  storageThread.setInterval(1000 * 10);

  setupDatabase();
}

void setBac() {
  sensorRead = analogRead(SENSOR_PIN);

  if (sensorRead < BAC_START) {
    bac = 0;
    displayedBac = String("000");
    return;
  }
  
  sensorRead       = sensorRead - BAC_START;
  uint8_t newBac   = BAC_CHART[sensorRead];
  bool bacIsHigher = newBac > bac;
  bac              = newBac;
  displayedBac     = formatBac(bac);

  if (bacIsHigher && storageThread.shouldRun()) {
    storageThread.run();
  }
}

void displayBac() {
  lcd.display(displayedBac);
}

void recordScore() {
  // TODO: capture initials
  HighScore newScore = { "AAA", 102 };
  writeScore(newScore);
}

void loop() {
  if (bacThread.shouldRun()) {
    bacThread.run();
  }

  if (displayThread.shouldRun()) {
    displayThread.run();
  }

  // send 's' to show scores
  if (Serial.available() > 0 && Serial.read() == 's') {
    printScores();
  }
}

