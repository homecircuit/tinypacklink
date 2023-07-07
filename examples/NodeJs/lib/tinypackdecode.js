function decodeTinypacklink(messageArray, messageLength) {
  let param = 0;

  if (messageArray[0] === STX) { // Verify the start byte
    param = (messageArray[1] << 8) | messageArray[2];

    if (param === Param.sourceId) { // Filter sourceID
      param = (messageArray[3] << 8) | messageArray[4];

      if (param === Param.destinationId) { // Filter target ID
        // Calculation LEN
        //[Message ID, Version code, Timestamp, componentId, payload]
        //    (1byte)     (1byte)     (4byte)     (1byte)     (n0..byte)
        //------------------------------------------
        //        Header message
        // [STX,  sourceId, destinationId, len]
        // (1byte) (2byte)     (2byte)   (1byte)
        //------------------------------------------
        //   CRC (2byte)

        // Calculate the CRC16 checksum of the received packet
        const payloadLength = messageArray[5]; // Message payload Length
        const receivedChecksum = calculateCRC16(messageArray.slice(6, 6 + payloadLength));
        const mesgChecksum = (messageArray[messageLength - 2] << 8) | messageArray[messageLength - 1];

        if (receivedChecksum === mesgChecksum) {
          // Process the valid packet
          // Add your logic here to handle the received packet
          // Use packet.sourceId, packet.destinationId, packet.messageId, etc.
        } else {
          // Invalid checksum, discard the packet
        }
      } else {
        // Invalid target, discard the packet
      }
    }
  } else {
    // Invalid start byte, discard the packet
  }
}
