#include <Thread.h>
#include <Wire.h>
#include "FourDigitLCD.h"
#include "BAC_Lookup.h"

#define SENSOR_PIN  A5
#define S7_ADDR     0x71
#define DEC_MASK    0b00000001
#define BAC_START   410     // Beginning ADC value of BAC chart
#define BAC_END     859     // Last ADC value in BAC chart

char temp_str[10];
int sensor_read;
uint8_t bac;
FourDigitLCD lcd;
String displayedBac = String("000");
Thread bacThread = Thread();
Thread displayThread = Thread();

void setup() {
  lcd.init();
  pinMode(SENSOR_PIN, INPUT);
  
  bacThread.onRun(setBac);
  bacThread.setInterval(100);

  displayThread.onRun(displayBac);
  displayThread.setInterval(1);
}

void setBac() {
  sensor_read = analogRead(SENSOR_PIN);

  if (sensor_read < BAC_START) {
      bac = 0;
      displayedBac = String("000");
  } else {
      sensor_read = sensor_read - BAC_START;
      bac = bac_chart[sensor_read];
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

