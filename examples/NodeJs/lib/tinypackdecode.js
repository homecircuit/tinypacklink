const STX_MSG = 0x55;
const CODE_VERSION = 0x01;
const MSG_TINYPACK_HEARTBEAT = 0x01;
const COMPONENT_ID_LAMP = 0x02;

function packTimestamp(year, month, day, hour) {
  // Pack the timestamp components into a 32-bit unsigned integer
  var packedTimestamp = (year << 24) | (month << 16) | (day << 8) | hour;
  return packedTimestamp;
}

// Function to convert hexadecimal string to byte array
function hexStringToByteArray(hexString, byteArraySize) {
  var len = hexString.length;
  var byteArray = new Uint8Array(byteArraySize);
  for (let i = 0; i < byteArraySize && i * 2 < len; ++i) {
    const hex = hexString.substr(i * 2, 2);
    byteArray[i] = parseInt(hex, 16);
  }
  return byteArray;
}

function calculateCRC16(data) {
  let crc = 0xFFFF;

  for (let i = 0; i < data.length; i++) {
    crc ^= data[i];

    for (let j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc = crc >> 1;
      }
    }
  }

  return crc;
}

var TinyPackLink = {
  stx: STX_MSG,
  sourceId: 0,
  destinationId: 0,
  len: 0,
  messageId: 0,
  code: CODE_VERSION,
  timestamp: 0,
  componentId: 0,
  payload: new Uint8Array(35), // Maximum of 35 bytes to accommodate the component ID
  crc16: 0,
};

function decodeTinypacklink(messageArray) {
  var tinypack = Object.assign({}, TinyPackLink);

  if (messageArray[0] === STX_MSG) { // Verify the start byte
    tinypack.stx = messageArray[0]; // Start of packet byte
    tinypack.sourceId = (messageArray[1] << 8) | messageArray[2];
    tinypack.destinationId = (messageArray[3] << 8) | messageArray[4];
    tinypack.len = messageArray[5];
    tinypack.messageId = messageArray[6];
    tinypack.code = messageArray[7];
    tinypack.timestamp = packTimestamp(
      messageArray[8],
      messageArray[9],
      messageArray[10],
      messageArray[11]
    );
    tinypack.componentId = messageArray[12];
    const crc_position = 12 + tinypack.len + 1;

    tinypack.crc16 = (messageArray[crc_position] << 8) | messageArray[crc_position + 1];

    // Calculate the CRC16 checksum of the received packet
    var receivedChecksum = calculateCRC16(messageArray.slice(0, 13 + tinypack.len));
    if (receivedChecksum === tinypack.crc16) {
      // Process the valid packet
      // Add your logic here to handle the received packet
      // Use tinypack.sourceId, tinypack.destinationId, tinypack.messageId, etc.
      // Unpack the payload data
      console.log("[DEBUG] *** tinypack  decode **");
      console.log("[DEBUG] sourceId: " + tinypack.sourceId.toString(16));
      console.log("[DEBUG] destinationId: " + tinypack.destinationId.toString(16));
      console.log("[DEBUG] len: " + tinypack.len);
      console.log("[DEBUG] messageId: " + tinypack.messageId);
      console.log("[DEBUG] code: " + tinypack.code);
      console.log("[DEBUG] timestamp: " + tinypack.timestamp);
      console.log("[DEBUG] componentId: " + tinypack.componentId);
      console.log("[DEBUG] crc16: " + tinypack.crc16.toString(16));
      console.log("[DEBUG] payload: \n----------------");
      for (let i = 0; i < tinypack.len; i++) {
        tinypack.payload[i] = messageArray[13 + i];
        console.log(" " + tinypack.payload[i]);
      }
      console.log("\n----------------");
    } else {
      // Invalid checksum, discard the packet
    }
  } else {
    // Invalid start byte, discard the packet
  }
}
