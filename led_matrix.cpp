
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
#include <SPI.h>

#define IMMEDIATE_NEW 0 // if 1 will immediately display a new message
#define USE_POT_CONTROL 0
#define PRINT_CALLBACK 0

#define PRINT(s, v)                                                            \
  {                                                                            \
    Serial.print(F(s));                                                        \
    Serial.print(v);                                                           \
  }

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN D5  // or SCK
#define DATA_PIN D7 // or MOSI
#define CS_PIN D8   // or SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary pins
// MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN,
// MAX_DEVICES);

// Scrolling parameters
#if USE_POT_CONTROL
#define SPEED_IN A5
#else
#define SCROLL_DELAY 75 // in milliseconds
#endif                  // USE_POT_CONTROL

#define CHAR_SPACING 1 // pixels between characters

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE 75
uint8_t curMessage[BUF_SIZE] = {"  "};
uint8_t newMessage[BUF_SIZE];
bool newMessageAvailable = false;

uint16_t scrollDelay; // in milliseconds

void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
// Callback function for data that is being scrolled off the display
{
#if PRINT_CALLBACK
  Serial.print("\n cb ");
  Serial.print(dev);
  Serial.print(' ');
  Serial.print(t);
  Serial.print(' ');
  Serial.println(col);
#endif
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
// Callback function for data that is required for scrolling into the display
{
  static uint8_t *p = curMessage;
  static enum {
    NEW_MESSAGE,
    LOAD_CHAR,
    SHOW_CHAR,
    BETWEEN_CHAR
  } state = LOAD_CHAR;
  static uint8_t curLen, showLen;
  static uint8_t cBuf[15];
  uint8_t colData = 0; // blank column is the default

#if IMMEDIATE_NEW
  if (newMessageAvailable) // there is a new message waiting
  {
    state = NEW_MESSAGE;
    mx.clear(); // clear the display
  }
#endif

  // finite state machine to control what we do on the callback
  switch (state) {
    case NEW_MESSAGE:                           // Load the new message
      memcpy(curMessage, newMessage, BUF_SIZE); // copy it in
      newMessageAvailable = false;              // used it!
      p = curMessage;
      state = LOAD_CHAR;
      break;

    case LOAD_CHAR: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state = SHOW_CHAR;

      // if we reached end of message, opportunity to load the next
      if (*p == '\0') {
        p = curMessage; // reset the pointer to start of message
#if !IMMEDIATE_NEW
        if (newMessageAvailable) // there is a new message waiting
        {
          state = NEW_MESSAGE; // we will load it here
          break;
        }
#endif
      }
      // !! deliberately fall through to next state to start displaying

    case SHOW_CHAR: // display the next part of the character
      colData = cBuf[curLen++];
      if (curLen == showLen) {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = BETWEEN_CHAR;
      }
      break;

    case BETWEEN_CHAR: // display inter-character spacing (blank columns)
      colData = 0;
      curLen++;
      if (curLen == showLen)
        state = LOAD_CHAR;
      break;

    default:
      state = LOAD_CHAR;
  }

  return (colData);
}

void scrollText(void) {
  static uint32_t prevTime = 0;

  // Is it time to scroll the text?
  if (millis() - prevTime >= scrollDelay) {
    mx.transform(
        MD_MAX72XX::TSL); // scroll along - the callback will load all the data
    prevTime = millis();  // starting point for next time
  }
}

uint16_t getScrollDelay(void) {
#if USE_POT_CONTROL
  uint16_t t;

  t = analogRead(SPEED_IN);
  t = map(t, 0, 1023, 25, 250);

  return (t);
#else
  return (SCROLL_DELAY);
#endif
}

namespace LedMatrix {

void setup() {
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);

#if USE_POT_CONTROL
  pinMode(SPEED_IN, INPUT);
#else
  scrollDelay = SCROLL_DELAY;
#endif

  newMessage[0] = '\0';

  Serial.print("\n[MD_MAX72XX Message Display]\nType a message for the "
               "scrolling display\nEnd message line with a newline");

  mx.update(MD_MAX72XX::OFF);
}

void loop() {}

void set_message(const String &str) {
  newMessageAvailable = true;
  int len = str.length();
  if (len >= BUF_SIZE) {
    len = BUF_SIZE - 1;
  }
  for (int i = 0; i < len; ++i) {
    newMessage[i] = str[i];
  }
  newMessage[len] = 0;
}

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

void set_point(const int col, const int row, const int state) { mx.setPoint(row, col, state); }

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
