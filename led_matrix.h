#include <MD_MAX72xx.h>


namespace LedMatrix{
  void setup();
  void loop();

  /* void set_message(const String &str); */
  void set_rows(int start_y, int count, int state);
  void set_cols(int start_x, int count, int state);
  void set_point(int col, int row, int state);
  void fill(bool on);
  void update();
}
