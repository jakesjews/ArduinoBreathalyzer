#include <Thread.h>
#include <Wire.h>
#include "FourDigitLCD.h"
#include "BacLookup.h"
#include "Database.h"

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

  setupDatabase();
}

void setBac() {
  sensorRead = analogRead(SENSOR_PIN);

  if (sensorRead < BAC_START) {
      bac = 0;
      displayedBac = String("000");
  } else {
      sensorRead = sensorRead - BAC_START;
      bac = BAC_CHART[sensorRead];
      String prefix = String("");
      if (bac < 10) {
        prefix = String("00");
      } else if (bac < 100) {
        prefix = String("0");
      }
      displayedBac = String(prefix + String(bac));
  }
}

void displayBac() {
  lcd.display(displayedBac);
}

void loop() {
  if (bacThread.shouldRun()) {
    bacThread.run();
  }

  if (displayThread.shouldRun()) {
    displayThread.run();
  }
}

