#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Sapido_BRF71n_d0c01a";
const char* password = "";

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266! EXEXEXEXEX101010101");
  delay(1000);
  digitalWrite(led, 0);
}

void handleA0() {

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/json" , String("{\"A0\":") + analogRead(A0) + "}");
}

void handleBlink() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "BLINK__BLINK_");

  for (int i = 0; i < 10; i++ ) {
    digitalWrite(led, 1);
    delay(500);
    digitalWrite(led, 0);
    delay(500);
  }
}

void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
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

  //  if (MDNS.begin("esp8266")) {
  //    Serial.println("MDNS responder started");
  //  }

  server.on("/", handleRoot);

  server.on("/blink", handleBlink);
  server.on("/a0", handleA0);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}