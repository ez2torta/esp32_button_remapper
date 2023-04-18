#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *ssid = "Despierta";
const char *password = "Q6HXEcd4";

WebServer server(80);
// default capcom setup
int playerOneOutputPinsSize = 6;

/*
**1P Side (Inputs)**
- BUTTON 1 = GPIO 32 (D32)
- BUTTON 2 = GPIO 33 (D33)
- BUTTON 3 = GPIO 34 (D34)
- BUTTON 4 = GPIO 35 (D35)
- BUTTON 5 = GPIO 36 (D26)
- BUTTON 6 = GPIO 39 (D27)
*/
int playerOneInputPins[6] = {32, 33, 34, 35, 36, 39};

/*
**1P Side (Outputs)**
- BUTTON 1 = GPIO 18 (D18)
- BUTTON 2 = GPIO 19 (D19)
- BUTTON 3 = GPIO 21 (D21)
- BUTTON 4 = GPIO 22 (D22)
- BUTTON 5 = GPIO 23 (D23)
- BUTTON 6 = GPIO 25 (D25)
- COIN = GPIO 26 (D26)
- SERVICE = GPIO 13 (D13)
- TEST = GPIO 14 (D14)
*/
int playerOneOutputPins[6] = {18, 19, 21, 22, 23, 25};
int serviceOutputPins[3] = {26, 13, 14};

void setup()
{
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize mDNS
  if (!MDNS.begin("arcade"))
  {
    Serial.println("Error setting up mDNS!");
  }
  else
  {
    Serial.println("mDNS responder started");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());
  // Start the server
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Server started");

  // Configure the 1P input pins
  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    pinMode(playerOneInputPins[i], INPUT);
  }

  // Configure the 1P output pins
  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    pinMode(playerOneOutputPins[i], OUTPUT);
  }
}

// Web Server!

void handleRoot()
{
  // Handle the root page
  String json = server.arg("plain");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  // print 1P old pins
  Serial.println("Old Output Pins For 1P:");

  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    Serial.print(playerOneOutputPins[i]);
    Serial.print(", ");
  }
  Serial.println("----------------");

  // Update Arrays
  if (doc["playerOneOutputPins"])
  {
    for (int i = 0; i < doc["playerOneOutputPins"].size(); i++)
    {
      playerOneOutputPins[i] = doc["playerOneOutputPins"][i].as<int>();
    }
  }
  // print 1P New pins
  Serial.println("New Output Pins For 1P:");

  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    Serial.print(playerOneOutputPins[i]);
    Serial.print(", ");
  }
  Serial.println("");
  server.send(200, "text/html", "Pin configuration updated");

  Serial.println("UPDATE SUCESSFUL");
}

void handleNotFound()
{
  // Handle 404 pages
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println("404 NOT FOUND");
}

void loop()
{
  server.handleClient();

  // Read the input pins and write the values to the output pins
  for (int i = 0; i < 6; i++)
  {
    int inputValue = digitalRead(playerOneInputPins[i]);
    digitalWrite(playerOneOutputPins[i], inputValue);
  }
}
