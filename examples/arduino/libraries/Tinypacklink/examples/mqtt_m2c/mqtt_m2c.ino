#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// MQTT broker details
const char* mqttServer = "MQTTBrokerIP";
const int mqttPort = 1883;
const char* mqttUsername = "YourMQTTUsername";
const char* mqttPassword = "YourMQTTPassword";
const char* mqttClientId = "esp32-client";

// TinyPackLink struct
struct TinyPackLink {
  uint8_t stx;               // Start of packet byte
  uint16_t sourceId;         // Source ID
  uint16_t destinationId;    // Destination ID
  uint8_t len;               // Length of payload
  uint8_t messageId;         // Message ID
  uint8_t code;              // Version code
  uint32_t timestamp;        // Timestamp
  uint8_t componentId;       // Component ID
  uint8_t payload[35];       // Payload data (maximum of 35 bytes to accommodate the component ID)
  uint16_t crc16;            // CRC16 checksum
};

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void connectToMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);

  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientId, mqttUsername, mqttPassword)) {
      Serial.println("Connected to MQTT broker!");
      mqttClient.subscribe("lamp/control");
    } else {
      Serial.print("Failed to connect to MQTT broker. Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages here
  // Assuming the received payload contains the TinyPackLink message
  TinyPackLink message;
  memcpy(&message, payload, sizeof(message));
  decodeTinypacklink(message.payload, message.len);
}

void decodeTinypacklink(const uint8_t* message_array, uint8_t messageLength) {
  if (message_array[0] == STX) { // Verify the start byte
    // Check the length of the message
    if (messageLength >= 10) {
      // Calculate the CRC16 checksum of the received packet
      uint16_t receivedChecksum = calculateCRC16((uint8_t*)&message_array[1], messageLength - 3);  // Exclude STX, CRC16 bytes
      uint16_t mesgChecksum = (message_array[messageLength - 2] << 8) | message_array[messageLength - 1];
      if (receivedChecksum == mesgChecksum) { // Verify the checksum
        // Process the valid packet
        uint16_t sourceId = (message_array[1] << 8) | message_array[2];
        uint16_t destinationId = (message_array[3] << 8) | message_array[4];
        uint8_t messageId = message_array[5];
        uint32_t timestamp = (message_array[6] << 24) | (message_array[7] << 16) | (message_array[8] << 8) | message_array[9];
        uint16_t crc16 = (message_array[messageLength - 3] << 8) | message_array[messageLength - 2];

        // Add your logic here to handle the received packet
        // Use sourceId, destinationId, messageId, timestamp, crc16 as needed
      } else {
        // Invalid checksum, discard the packet
      }
    } else {
      // Invalid message length, discard the packet
    }
  } else {
    // Invalid start byte, discard the packet
  }
}

// Function to calculate CRC16 checksum
uint16_t calculateCRC16(const uint8_t* data, size_t length) {
  uint16_t crc = 0xFFFF;

  for (size_t i = 0; i < length; i++) {
    crc ^= data[i];

    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc = crc >> 1;
      }
    }
  }

  return crc;
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();
  // Additional code can be added here as per your requirements
}
