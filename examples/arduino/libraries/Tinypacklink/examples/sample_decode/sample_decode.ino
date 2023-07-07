void decodeTinypacklink(const uint8_t *message_arry,uint8_t messageLength){
  uint32_t param = 0;
  if(message_arry[0] == STX){ // Verify the start byte
    
    param =  (uint32_t)(message_arry[1]<<8) | message_arry[2];
    if(param == Param.sourceId){ //filter sourceID
      
      param =  (uint32_t)(message_arry[3]<<8) | message_arry[4];
      if(param == Param.destinationId){//filter target ID
        //------------------------------------------
        //        Calculation LEN
        //[Message ID, Version code, Timestamp, componentId, payload]
        //    (1byte)     (1byte)     (4byte)     (1byte)     (n0..byte)
        //------------------------------------------
        //        Headder message
        // [STX,  sourceId, destinationId, len]
        // (1byte) (2byte)     (2byte)   (1byte)
        //------------------------------------------
        //   CRC (2byte)


        // Calculate the CRC16 checksum of the received packet
        uint16_t receivedChecksum = calculateCRC16((uint8_t*)&message_arry[6], message_arry[5]);  //message_arry[5]:message payload Length
        uint16_t mesgChecksum = (message_arry[messageLength-2]<<8)|message_arry[messageLength-1];
        if(receivedChecksum == mesgChecksum){ //
        // Process the valid packet
        // Add your logic here to handle the received packet
        // Use packet.sourceId, packet.destinationId, packet.messageId, etc.
        }else{

        }

      }else{
         // Invalid target, discard the packet
      }
    }

  }else {
        // Invalid checksum, discard the packet
  }
}
