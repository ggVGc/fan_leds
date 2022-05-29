#include "led_matrix.h"

struct LedPos {
  int x;
  int y;
};

struct State {
  static const int MAX = 64;

  LedPos blinked[MAX];

  void init() { counter = 0; }

  bool add(const int x, const int y) {
    if (counter < MAX) {
      blinked[counter].x = x;
      blinked[counter].y = y;
      ++counter;
      return true;
    }

    return false;
  }

  int counter;
};

static State state;

namespace Blinker {
void setup() { state.init(); }

void loop() {
  if (state.counter > 0) {
    for (int i = 0; i < state.counter; ++i) {
      const LedPos &entry = state.blinked[i];
      LedMatrix::set_point(entry.x, entry.y, 0);
    }
    state.init();
  }
}

void blink(const int x, const int y) {
  if (state.add(x, y)) {
    LedMatrix::set_point(x, y, 1);
  }
}
} // namespace Blinker
