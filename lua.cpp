#include "led_matrix.h"
#include <LuaWrapper.h>
#include "blinker.h"

LuaWrapper lua;

static int lua_pinMode(lua_State *lua_state) {
  int a = luaL_checkinteger(lua_state, 1);
  int b = luaL_checkinteger(lua_state, 2);
  pinMode(a, b);
  return 0;
}

static int lua_builtin_led(lua_State *lua_state) {
  const int state = luaL_checkinteger(lua_state, 1);
  digitalWrite(LED_BUILTIN, state);
  return 0;
}

static int lua_digitalWrite(lua_State *lua_state) {
  int a = luaL_checkinteger(lua_state, 1);
  int b = luaL_checkinteger(lua_state, 2);
  digitalWrite(a, b);
  return 0;
}

/*
static int lua_delay(lua_State *lua_state) {
  int a = luaL_checkinteger(lua_state, 1);
  delay(a);
  return 0;
}
*/

static int lua_millis(lua_State *lua_state) {
  lua_pushnumber(lua_state, (lua_Number)millis());
  return 1;
}

static int lua_set_point(lua_State *lua_state) {
  const int x = luaL_checkinteger(lua_state, 1);
  const int y = luaL_checkinteger(lua_state, 2);
  const int state = luaL_checkinteger(lua_state, 3);

  LedMatrix::set_point(x, y, state);

  return 0;
}

static int lua_set_cols(lua_State *lua_state) {
  const int start_x = luaL_checkinteger(lua_state, 1);
  const int count = luaL_checkinteger(lua_state, 2);
  const int state = luaL_checkinteger(lua_state, 3);

  LedMatrix::set_cols(start_x, count, state);

  return 0;
}

static int lua_fill_display(lua_State *lua_state) {
  LedMatrix::fill(true);
  return 0;
}

static int lua_set_rows(lua_State *lua_state) {
  const int start_y = luaL_checkinteger(lua_state, 1);
  const int count = luaL_checkinteger(lua_state, 2);
  const int state = luaL_checkinteger(lua_state, 3);

  LedMatrix::set_rows(start_y, count, state);
  return 0;
}

static int lua_clear_display(lua_State *lua_state) {
  LedMatrix::fill(false);
  return 0;
}

static int lua_end_frame(lua_State *lua_state) {
  LedMatrix::update();
  return 0;
}

static int lua_blink_point(lua_State *lua_state) {
  const int x = luaL_checkinteger(lua_state, 1);
  const int y = luaL_checkinteger(lua_state, 2);

  Blinker::blink(x, y);
  return 0;
}

void lua_setup() {
  //lua.Lua_register("delay", (const lua_CFunction)&lua_delay);

  lua.Lua_register("pinMode", (const lua_CFunction)&lua_pinMode);
  lua.Lua_register("digitalWrite", (const lua_CFunction)&lua_digitalWrite);
  lua.Lua_register("millis", (const lua_CFunction)&lua_millis);

  lua.Lua_register("set_builtin_led", (const lua_CFunction)&lua_builtin_led);
  lua.Lua_register("set_point", (const lua_CFunction)&lua_set_point);
  lua.Lua_register("clear_display", (const lua_CFunction)&lua_clear_display);
  lua.Lua_register("fill_display", (const lua_CFunction)&lua_fill_display);
  lua.Lua_register("set_cols", (const lua_CFunction)&lua_set_cols);
  lua.Lua_register("set_rows", (const lua_CFunction)&lua_set_rows);
  lua.Lua_register("end_frame", (const lua_CFunction)&lua_end_frame);
  lua.Lua_register("blink_point", (const lua_CFunction)&lua_blink_point);
}

void lua_run_script(const String &script) {
  const auto res = lua.Lua_dostring(&script);
}
