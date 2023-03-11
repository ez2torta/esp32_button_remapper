#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *ssid = "Despierta";
const char *password = "Q6HXEcd4";

WebServer server(80);
// default capcom setup
int playerOneOutputPinsSize = 6;
int playerTwoOutputPinsSize = 6;

/*
**1P Side (Inputs)**
- GPIO 3 (RX0)
- GPIO 22 (D22)
- GPIO 23 (D23)
- GPIO 25 (D25)
- GPIO 26 (D26)
- GPIO 27 (D27)
*/
int playerOneInputPins[6] = {3, 22, 23, 25, 26, 27};
/*
**2P Side (Inputs)**
- GPIO 32 (D32)
- GPIO 33 (D33)
- GPIO 34 (D34)
- GPIO 35 (D35)
- GPIO 36 (VP)
- GPIO 39 (VN)
*/
int playerTwoInputPins[6] = {32, 33, 34, 35, 36, 39};
/*
**1P Side (Outputs)**
- GPIO 2 (D2)
- GPIO 4 (D4)
- GPIO 5 (D5)
- GPIO 12 (D12)
- GPIO 13 (D13)
- GPIO 16 (RX2)
*/
int playerOneOutputPins[6] = {2, 4, 5, 12, 13, 16};
/*
**2P Side (Outputs)**

- GPIO 17 (TX2)
- GPIO 18 (D18)
- GPIO 19 (D19)
- GPIO 1 (TX0)
- GPIO 21 (D21)
- GPIO 14 (D14)
*/
int playerTwoOutputPins[6] = {17, 18, 19, 1, 21, 14};

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
  Serial.println("");

  // print 2P old pins
  Serial.println("Old Output Pins For 2P:");

  for (int i = 0; i < playerTwoOutputPinsSize; i++)
  {
    Serial.print(playerTwoOutputPins[i]);
    Serial.print(", ");
  }
  Serial.println("");
  // Update Arrays
  if (doc["playerOneOutputPins"])
  {
    for (int i = 0; i < doc["playerOneOutputPins"].size(); i++)
    {
      playerOneOutputPins[i] = doc["playerOneOutputPins"][i].as<int>();
    }
  }
  if (doc["playerTwoOutputPins"])
  {
    for (int i = 0; i < doc["playerTwoOutputPins"].size(); i++)
    {
      playerTwoOutputPins[i] = doc["playerTwoOutputPins"][i].as<int>();
    }
  }
  // print new pins (to check if they changed)

  // print 1P New pins
  Serial.println("New Output Pins For 1P:");

  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    Serial.print(playerOneOutputPins[i]);
    Serial.print(", ");
  }
  Serial.println("");
  // print 2P New pins
  Serial.println("New Output Pins For 2P:");

  for (int i = 0; i < playerTwoOutputPinsSize; i++)
  {
    Serial.print(playerTwoOutputPins[i]);
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

  // Configure the 2P input pins
  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    pinMode(playerOneInputPins[i], INPUT);
  }

  // Configure the 2P output pins
  for (int i = 0; i < playerOneOutputPinsSize; i++)
  {
    pinMode(playerOneOutputPins[i], OUTPUT);
  }
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
  for (int i = 0; i < 6; i++)
  {
    int inputValue = digitalRead(playerTwoInputPins[i]);
    digitalWrite(playerTwoOutputPins[i], inputValue);
  }
}
