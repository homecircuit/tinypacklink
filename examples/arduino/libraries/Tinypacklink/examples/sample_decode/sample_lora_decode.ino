#include <Arduino.h>
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
    uint8_t crc_position = 12 + tinypack.len + 1;

    tinypack.crc16 = (messageArray[crc_position] << 8) | messageArray[crc_position + 1];

    // Calculate the CRC16 checksum of the received packet
    uint16_t receivedChecksum = calculateCRC16(messageArray, 13 + tinypack.len);
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

int8_t checkATCommand(const String input_string, const char* at_command_prefix) {
  uint8_t prefix_length = strlen(at_command_prefix);
  uint8_t string_length = input_string.length();

  // Ensure that the input string is long enough to contain the prefix and the comma-separated numbers
  if (string_length < prefix_length + 7) { // Minimum length: prefix + 3 comma-separated numbers + colon
    return -1; // Return -1 if the input string is too short
  }

  // Check if the prefix matches the expected AT command prefix
  if (input_string.startsWith(at_command_prefix)) {
    // Check if the character right after the prefix is a comma
    if (input_string.charAt(prefix_length+1) == ',') {
      // Find the positions of the commas after the numbers
      int8_t comma1_index = input_string.indexOf(',', prefix_length + 1);
      int8_t comma2_index = input_string.indexOf(',', comma1_index + 1);
      int8_t comma3_index = input_string.indexOf(':', comma2_index + 1);

      // Check if all commas and the colon are found at expected positions
      if (comma1_index != -1 && comma2_index != -1 && comma3_index != -1) {
        // Check if the characters between commas are digits (numbers)
        if (input_string.substring(prefix_length , comma1_index).toInt() &&
            input_string.substring(comma1_index + 1, comma2_index).toInt() &&
            input_string.substring(comma2_index + 1, comma3_index).toInt()) {
          return comma3_index + 1; // Return the index of ":" + 1
        }
      }
    }
  }
  return -1; // Return -1 if the command is not found or does not meet the formatting requirements
}

void setup() {
  Serial.begin(115200);
}

uint8_t buff[128];
void loop() {
  // AT command prefix to search for
  String ack = "at+recv=2,-98,7,37:55888800011601015de4f1d8026464646405a06405a06405a06405a06405a06405a0648a48";



  // AT command prefix to search for
  const char* at_command_prefix = "at+recv=";
  int16_t command_index = checkATCommand(ack, at_command_prefix);
  // Serial.println("command_index: " + String(command_index));

  // Serial.println("---------------------------");
  // for(uint8_t i=0;i<ack.length()/2;i++){
  //   Serial.println(" ,"+String(buff[command_index+i]));
  // }
  // Serial.println("\n---------------------------");

  if (command_index != -1) {
    // Print the index of ":" + 1 in the byte array
    String command_data = ack.substring(command_index);
    hexStringToByteArray(command_data,buff,command_data.length());
  
    // Serial.println("Command Data: " + command_data);
    decodeTinypacklink(buff, (uint8_t)(command_data.length()/2)); //2char ==> 1byte 
  }

  delay(5000);
}
