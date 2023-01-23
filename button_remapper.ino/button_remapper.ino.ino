#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Despierta";
const char* password = "Q6HXEcd4";

WebServer server(80);
// default 1P capcom setup
int outputPinsSize = 6;
int inputPins[6] = {3,22,23,25,26,27};
int outputPins[6] = {2,4,5,12,13,16};

void handleRoot() {
  // Handle the root page
  String json = server.arg("plain");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  // print old pins
  Serial.println("Old Output Pins:");

  for(int i = 0; i < outputPinsSize; i++){
    Serial.println(outputPins[i]);
  }

  for(int i = 0; i < doc["outputPins"].size(); i++){
    outputPins[i] = doc["outputPins"][i].as<int>();
  }
  // print new pins (to check if they changed)
  Serial.println("New Pins:");
  for(int i = 0; i < outputPinsSize; i++){
    Serial.println(outputPins[i]);
  }
  server.send(200, "text/html", "Pin configuration updated");
  Serial.println("UPDATE SUCESSFUL");
}

void handleNotFound() {
  // Handle 404 pages
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
  Serial.println("404 NOT FOUND");
}

void setup() {
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the server
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Server started");

  // Configure the input pins
  for (int i = 0; i < outputPinsSize; i++) {
    pinMode(inputPins[i], INPUT);
  }

  // Configure the output pins
  for (int i = 0; i < outputPinsSize; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
}

void loop() {
  server.handleClient();

  // Read the input pins and write the values to the output pins
  for (int i = 0; i < 6; i++) {
    int inputValue = digitalRead(inputPins[i]);
    digitalWrite(outputPins[i], inputValue);
  }
}

