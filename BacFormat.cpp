#include "BacFormat.h"

String formatBac(uint8_t score) {
  String prefix = String("");
  if (score < 10) {
    prefix = String("00");
  } else if (score < 100) {
    prefix = String("0");
  }
  return String(prefix + String(score));
}
