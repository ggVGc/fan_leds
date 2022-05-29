#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include "server.h"

const char *ssid = "Gorzy2";                        // Enter your WIFI ssid
const char *password = "cremefraiche&156chipsdip;"; // Enter your WIFI password
                                                    //
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

static MessageType message_type = None;
static String message;

static void handleOneShot() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    message_type = OneShot;
    message = server.arg("plain");
    server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
  }
}

static void handleSetLoop() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    message_type = Loop;
    message = server.arg("plain");
    server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
  }
}

static void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup_server() {
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/oneshot/", handleOneShot);
  server.on("/loop/", handleSetLoop);

  server.onNotFound(handleNotFound);

  httpUpdater.setup(&server);
  MDNS.addService("http", "tcp", 80);
  server.begin();
  Serial.println("HTTP server started");
}

void server_loop_step() { server.handleClient(); }

MessageType get_message_type() { return message_type; }

const String &get_message() {
  message_type = None;
  return message;
}
