#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "details.h"

// actual topic dynamically added in setup below 
// NOTE don't forgt to upate the details.h file
char mqtt_cmd_topic[50];  // Adjust size as needed
char mqtt_status_topic[50];  // Adjust size as needed
char client_id[100];

// NeoPixel setup
#define PIN D2 // Data pin for NeoPixel
#define NUMPIXELS 12 // Number of LEDs in the NeoPixel ring
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Globals for managing state
unsigned long lastPeriodicTime = 0;
unsigned long periodicInterval = 15 * 60 * 1000; // 15 minutes periodic reporting for wifi experience
unsigned long modeStartTime = 0;
unsigned long modeDuration = 0;

int reconnection_count = 0;
int status_count = 0;
int req_id = 0;
String current_command = "none";

bool isModeActive = false;
String activeMode = "";

// Color variables
int red = 255, green = 255, blue = 255; // Default to white

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);


//****************************************************************************  
// Function to connect to Wifi
//**************************************************************************** 
void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

//**************************************************************************** 
// Function to reconnect MQTT
//**************************************************************************** 
void reconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi(); // Reconnect Wi-Fi if disconnected
  }

  while (!client.connected()) {
    reconnection_count++; // Increment reconnection count
    Serial.print("Connecting to MQTT...");
    Serial.println(client_id);
    if (client.connect(client_id)) {
      Serial.println("Connected to MQTT.");
      client.subscribe(mqtt_cmd_topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//****************************************************************************  
// Function to publish status
//**************************************************************************** 
void publishStatus(const char* status_type, int req_id) {
  int rssi = WiFi.RSSI(); // Get RSSI

  JsonDocument statusDoc;
  statusDoc["status_type"] = status_type;
  statusDoc["rssi"] = rssi;
  statusDoc["reconnection_count"] = reconnection_count;
  statusDoc["req_id"] = req_id;
  statusDoc["current_command"] = current_command;

  char statusMessage[256];
  serializeJson(statusDoc, statusMessage);
  client.publish(mqtt_status_topic, statusMessage);

  Serial.print("Published status: ");
  Serial.println(statusMessage);
}

//**************************************************************************** 
// Function to handle blink modes
//**************************************************************************** 
void handleMode(String mode) {
  if (mode == "on") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Set color
    }
    pixels.show();
  } else if (mode == "off") {
    pixels.clear();
    pixels.show();
  } else if (mode == "worm") {
    int position = (millis() / 100) % NUMPIXELS;
    pixels.clear();
    pixels.setPixelColor(position, pixels.Color(red, green, blue)); // Worm color
    pixels.show();
  } else if (mode == "flash") {
    int state = (millis() / 500) % 2; // Toggle every 500ms
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, state == 0 ? pixels.Color(red, green, blue) : pixels.Color(0, 0, 0)); // Flash color
    }
    pixels.show();
  }
}

//**************************************************************************** 
// callback - Function to handle incoming MQTT messages
//**************************************************************************** 
void callback(char* topic, byte* payload, unsigned int length) {

  // Parse incoming message
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.print("Message received: ");
  Serial.println(message);

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("JSON parse error: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract parameters
  const char* mode = doc["mode"];
  int seconds = doc["seconds"];
  req_id = doc["req_id"];
  red = doc["red"];
  green = doc["green"];
  blue = doc["blue"];

  // Update state
  activeMode = String(mode);
  current_command = activeMode;
  modeStartTime = millis();
  modeDuration = seconds * 1000;
  isModeActive = true;

  // Publish acknowledgment
  publishStatus("acknowledged", req_id);
}

//**************************************************************************** 
// handlePeriodicReporting - Function to handle periodic reporting
//**************************************************************************** 
void handlePeriodicReporting() {
  unsigned long currentTime = millis();
  if (currentTime - lastPeriodicTime >= periodicInterval) {
    lastPeriodicTime = currentTime;
    publishStatus("periodic", status_count);
      status_count++; // Increment request count

  }
}

//**************************************************************************** 
// setup
//**************************************************************************** 
void setup() {
  Serial.begin(115200);
  setup_wifi();

  // Use Wifi MAC for client id uniqueness
  String mac = WiFi.macAddress();
  mac.replace(":", "");  // Remove colons
  snprintf(client_id, sizeof(client_id), "%s_%s", base_topic, mac.c_str());

  snprintf(mqtt_cmd_topic, sizeof(mqtt_cmd_topic), "%s/cmd", base_topic);
  snprintf(mqtt_status_topic, sizeof(mqtt_status_topic), "%s/status", base_topic);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pixels.begin();
  pixels.clear();
}


//**************************************************************************** 
// loop
//**************************************************************************** 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Handle active mode
  if (isModeActive) {
    handleMode(activeMode);

    if (millis() - modeStartTime >= modeDuration) {
      isModeActive = false;
      activeMode = "off";
      handleMode("off");
      current_command = "none";
      publishStatus("completed", req_id);
    }
  }

  // Handle periodic reporting
  handlePeriodicReporting();
}
