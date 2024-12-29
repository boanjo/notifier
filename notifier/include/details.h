#ifndef DETAILS_H
#define DETAILS_H

// Remove this define ERROR once you have updated ALL the values below
#define ERROR


// WiFi credentials
const char* ssid = "PetesWifi";
const char* password = "bestpasswd";

// MQTT server details (might be same as your HomeAssistant or other)
const char* mqtt_server = "192.168.1.10";

// Note this is the topic that you control the device with
// I.e. to send command from example from HomeAsistant
const char* base_topic = "oliver_food";

#ifdef ERROR
#error "You must update all values in DETAILS_H and remove #define ERROR to proceed."
#endif

#endif // DETAILS_H
