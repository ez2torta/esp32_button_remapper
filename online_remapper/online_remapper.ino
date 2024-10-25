#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *ssid = "Despierta";
const char *password = "Q6HXEcd4";

WebServer server(80);
// default capcom setup
/*
**Button Input Side**
- BUTTON 1 = GPIO 32 (D32)
- BUTTON 2 = GPIO 33 (D33)
- BUTTON 3 = GPIO 34 (D34)
- BUTTON 4 = GPIO 35 (D35)
- BUTTON 5 = GPIO 36 (D26)
- BUTTON 6 = GPIO 39 (D27)
*/
int input_pins[6] = { 32, 33, 34, 35, 36, 39 };

// Button Input Combo (Can be whatever)
// D4 = ??? (Button Combo?)
// D5 = ??? (Button Combo?)

int button_combo_input_pins[2] = { 4, 5 };
/*
**Button Output Side**
- BUTTON 1 = GPIO 18 (D18)
- BUTTON 2 = GPIO 19 (D19)
- BUTTON 3 = GPIO 21 (D21)
- BUTTON 4 = GPIO 22 (D22)
- BUTTON 5 = GPIO 23 (D23)
- BUTTON 6 = GPIO 25 (D25)
*/
int output_pins[6] = { 18, 19, 21, 22, 23, 25 };
// SERVICE Output Pins
// - COIN = GPIO 26 (D26)
// - SERVICE = GPIO 13 (D13)
// - TEST = GPIO 14 (D14)
int service_output_pins[3] = { 26, 13, 14 };

void setup() {
  // Configure the input pins
  for (int i = 0; i < 6; i++) {
    pinMode(input_pins[i], INPUT);
  }
  delay(10);
  // Configure the output pins
  for (int i = 0; i < 6; i++) {
    pinMode(output_pins[i], OUTPUT);
    digitalWrite(output_pins[i], HIGH);
  }
  delay(10);
  // Service Output Pins
  for (int i = 0; i < 3; i++) {
    pinMode(service_output_pins[i], OUTPUT);
    digitalWrite(service_output_pins[i], HIGH);
  }
  delay(10);

  Serial.begin(115200);
  delay(100);
  while (!Serial)
    ;
  delay(3000);
  Serial.println("Ready.");
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize mDNS
  if (!MDNS.begin("tortimatch1p")) {
    Serial.println("Error setting up mDNS!");
  } else {
    Serial.println("mDNS responder started");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());
  // Start the server
  server.on("/", handleRoot);
  server.on("/myip", handleMyIp);
  server.on("/config", handleConfig);
  server.on("/credit", handleCredit);
  server.on("/test", handleTest);
  server.on("/service", handleService);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Server started");
}

void handleMyIp() {
  server.send(200, "text/html", WiFi.localIP().toString());
}
// handleCredit
void handleCredit() {
  // credit button press
  digitalWrite(service_output_pins[0], LOW);
  delay(100);
  digitalWrite(service_output_pins[0], HIGH);
  Serial.println("Credit Inserted");
  server.send(200, "text/html", "Credit Inserted");
}

void handleTest() {
  // test button
  digitalWrite(service_output_pins[2], LOW);
  delay(100);
  digitalWrite(service_output_pins[2], HIGH);
  Serial.println("Test Menu Button");
  server.send(200, "text/html", "Test Menu Button Pressed");
}

void handleService() {
  digitalWrite(service_output_pins[1], LOW);
  delay(100);
  digitalWrite(service_output_pins[1], HIGH);
  Serial.println("Service Menu Button");
  server.send(200, "text/html", "Service Menu Button Pressed");
}

void handleConfig() {
  // Handle the root page
  String json = server.arg("plain");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    server.send(500, "text/html", "Internal Server Error :(");
    return;
  }

  // Update Arrays
  if (doc["output_pins"]) {
    // print Old Output Pins
    Serial.println("Old Output Pins:");

    for (int i = 0; i < 6; i++) {
      Serial.print(output_pins[i]);
      if (i < 5) {
        Serial.print(", ");
      }
    }
    Serial.println("");
    Serial.println("----------------");
    for (int i = 0; i < doc["output_pins"].size(); i++) {
      output_pins[i] = doc["output_pins"][i].as<int>();
    }
    // print new Output Pins
    Serial.println("New Output Pins ");

    for (int i = 0; i < 6; i++) {
      Serial.print(output_pins[i]);
      if (i < 5) {
        Serial.print(", ");
      }
    }
    server.send(200, "text/html", "Pin configuration updated");
    Serial.println("");
  }
  return;
}

// Web Server!


void handleRoot() {
  // Handle the root page
  String json = server.arg("plain");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    server.send(500, "text/html", "Internal Server Error :(");
    return;
  }
  server.send(200, "text/html", "ESP32 Working!");
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

void loop() {
  server.handleClient();

  // Read the input pins and write the values to the output pins
  for (int i = 0; i < 6; i++) {
    digitalWrite(output_pins[i], digitalRead(input_pins[i]));
  }
}
