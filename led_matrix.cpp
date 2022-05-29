
// Use the MD_MAX72XX library to scroll text on the display
//
// Demonstrates the use of the callback function to control what
// is scrolled on the display text.
//
// User can enter text on the serial monitor and this will display as a
// scrolling message on the display.
// Speed for the display is controlled by a pot on SPEED_IN analog in.
//
#include "led_matrix.h"
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN D5  // or SCK
#define DATA_PIN D7 // or MOSI
#define CS_PIN D8   // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

namespace LedMatrix {

void setup() {
  mx.begin();
  mx.update(MD_MAX72XX::OFF);
}

void loop() {}

void update() { mx.update(); }

void set_rows(const int start_y, const int count, const int state) {
  for (int y = 0; y < count; ++y) {
    for (int x = 0; x < 32; ++x) {
      mx.setPoint(start_y + y, x, state);
    }
  }
}

void set_cols(const int start_x, const int count, const int state) {
  for (int x = 0; x < count; ++x) {
    for (int y = 0; y < 8; ++y) {
      mx.setPoint(y, start_x + x, state);
    }
  }
}

void set_point(const int col, const int row, const int state) {
  mx.setPoint(row, col, state);
}

void fill(const bool on) {
  if (on) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 8; ++y) {
        mx.setPoint(y, x, 1);
      }
    }
  } else {
    mx.clear();
  }
}

} // namespace LedMatrix
