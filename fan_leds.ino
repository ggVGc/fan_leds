#include "server.h"
#include "lua.h"
#include "led_matrix.h"
#include "blinker.h"


void setup(void) {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  setup_server();
  lua_setup();

  Blinker::setup();
  LedMatrix::setup();
}

static String loop_script = "";

void loop(void) {
  Blinker::loop();

  server_loop_step();

  const auto type = get_message_type();
  switch (type) {
    case OneShot: {
                    const auto msg = get_message();
                    /* Serial.println(msg); */
                    lua_run_script(msg);
                  }
                  break;
    case Loop:
                  {
                    const auto msg = get_message();
                    /* Serial.println(msg); */
                    loop_script = msg;
                  }
                  break;
    default:
                  break;
  }

  if (loop_script != "") {
    lua_run_script(loop_script);
  }


  LedMatrix::loop();
}

