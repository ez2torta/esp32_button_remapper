#include <Arduino.h>
#include <ArduinoJson.h>

const char* filename = "/pins.json";

JsonArray inputPins;
JsonArray outputPins;

void setup() {
  // Open the file for reading
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open pins.json");
    return;
  }

  // Allocate a buffer to store the contents of the file
  size_t size = file.size();
  DynamicJsonDocument doc(size);

  // Parse the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.print("Failed to parse pins.json: ");
    Serial.println(error.c_str());
    return;
  }

  // Close the file
  file.close();

  // Get the arrays of input and output pins from the JSON document
  inputPins = doc["inputPins"];
  outputPins = doc["outputPins"];

  // Configure the input pins
  for (int i = 0; i < inputPins.size(); i++) {
    pinMode(inputPins[i], INPUT);
  }

  // Configure the output pins
  for (int i = 0; i < outputPins.size(); i++) {
    pinMode(outputPins[i], OUTPUT);
  }
}

void loop() {
  // Read the input pins and write the values to the output pins
  for (int i = 0; i < inputPins.size(); i++) {
    int inputValue = digitalRead(inputPins[i]);
    digitalWrite(outputPins[i], inputValue);
  }
}