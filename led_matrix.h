#ifndef LED_MATRIX_H_M4JY8R3Y
#define LED_MATRIX_H_M4JY8R3Y

namespace LedMatrix {
void setup();
void loop();

/* void set_message(const String &str); */
void set_rows(int start_y, int count, int state);
void set_cols(int start_x, int count, int state);
void set_point(int col, int row, int state);
void fill(bool on);
void update();
} // namespace LedMatrix

#endif /* end of include guard: LED_MATRIX_H_M4JY8R3Y */
