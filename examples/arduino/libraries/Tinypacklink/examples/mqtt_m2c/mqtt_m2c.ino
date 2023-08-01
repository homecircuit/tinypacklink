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

#ifdef DEBUG
#define DEBUG_PRINT(x)    //Serial.print(x)
#define DEBUG_PRINTLN(x)  //Serial.println(x)
#else
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#endif

#define STX_MSG 0x55
#define CODE_VERSION 0x01
#define MSG_TINYPACK_HARTHBEAT 0x01
#define COMPONENT_ID_LAMP 0x02

typedef struct{
  uint8_t stx;               // Start of packet byte
  uint16_t sourceId;         // Source ID
  uint16_t destinationId;    // Destination ID
  uint8_t len;               // Length of payload
  uint8_t messageId;         // Message ID
  uint8_t code;              // Version code
  uint32_t timestamp;        // Timestamp
  uint8_t componentId;      // Component ID
  uint8_t payload[35];       // Payload data (maximum of 35 bytes to accommodate the component ID)
  uint16_t crc16;            // CRC16 checksum
}TinyPackLink_t;


uint32_t packTimestamp(uint8_t year, uint8_t month, uint8_t day, uint8_t hour) {
  // Pack the timestamp components into a 32-bit unsigned integer
  uint32_t packedTimestamp = ((uint32_t)year << 24) | ((uint32_t)month << 16) | ((uint32_t)day << 8) | (uint32_t)hour;

  return packedTimestamp;
}

// Function to convert hexadecimal string to byte array
void hexStringToByteArray(const String& hexString, uint8_t* byteArray, uint8_t byteArraySize) {
    size_t len = hexString.length();
    for (size_t i = 0; i < byteArraySize && i * 2 < len; ++i) {
        char hex[3];
        hex[0] = hexString[i * 2];
        hex[1] = hexString[i * 2 + 1];
        hex[2] = '\0';
        byteArray[i] = strtoul(hex, nullptr, 16);
    }
}

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

void decodeTinypacklink(const uint8_t *messageArray, uint8_t messageLength) {
  TinyPackLink_t tinypack;

  if (messageArray[0] == STX_MSG) { // Verify the start byte
    tinypack.stx = messageArray[0]; // Start of packet byte
    tinypack.sourceId = (uint16_t)(messageArray[1] << 8) | messageArray[2];
    tinypack.destinationId = (uint16_t)(messageArray[3] << 8) | messageArray[4];
    tinypack.len = messageArray[5];
    tinypack.messageId = messageArray[6];
    tinypack.code = messageArray[7];
    tinypack.timestamp = packTimestamp(messageArray[8], messageArray[9], messageArray[10], messageArray[11]);
    tinypack.componentId = messageArray[12];
    uint8_t crc_position = 6 + tinypack.len;

    tinypack.crc16 = (messageArray[crc_position] << 8) | messageArray[crc_position + 1];

    // Calculate the CRC16 checksum of the received packet
    uint16_t receivedChecksum = calculateCRC16(messageArray, 6 + tinypack.len);
    if (receivedChecksum == tinypack.crc16) {
      // Process the valid packet
      // Add your logic here to handle the received packet
      // Use tinypack.sourceId, tinypack.destinationId, tinypack.messageId, etc.
      // Unpack the payload data
      DEBUG_PRINTLN(F("[DEBUG] *** tinypack  decode **"));
      DEBUG_PRINTLN("[DEBUG] sourceId: "+ String(tinypack.sourceId,HEX));
      DEBUG_PRINTLN("[DEBUG] destinationId: "+ String(tinypack.destinationId,HEX));
      DEBUG_PRINTLN("[DEBUG] len: "+ String(tinypack.len));
      DEBUG_PRINTLN("[DEBUG] messageId: "+ String(tinypack.messageId));
      DEBUG_PRINTLN("[DEBUG] code: "+ String(tinypack.code));
      DEBUG_PRINTLN("[DEBUG] timestamp: "+ String(tinypack.timestamp));
      DEBUG_PRINTLN("[DEBUG] componentId: "+ String(tinypack.componentId));
      DEBUG_PRINTLN("[DEBUG] componentId: "+ String(tinypack.componentId));
      DEBUG_PRINTLN("[DEBUG] crc16: "+ String(tinypack.crc16,HEX));
      DEBUG_PRINTLN("[DEBUG] payload: \n----------------");
      for (uint8_t i = 0; i < tinypack.len; i++) {
        tinypack.payload[i] = messageArray[13 + i];
        DEBUG_PRINT(" "+String(tinypack.payload[i]));
      }
      DEBUG_PRINTLN("\n----------------");
    } else {
      // Invalid checksum, discard the packet
    }
  } else {
    // Invalid start byte, discard the packet
  }
}

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


uint8_t buf[128];
/* --------- Callback function get data from web ---------- */
void callback(String topic, byte* payload, unsigned int length) {
  //DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  DEBUG_PRINTLN(message);
  sysconfig.byteArraySize =  hexStringToByteArray(message,&buff[0]);
  decodeTinypacklink(buff,byteArraySize);
  
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
