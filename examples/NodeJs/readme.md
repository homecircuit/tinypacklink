<strong><h2><p><center>Tinypacklink protocol</center></p></h2></strong>
<pre>const TinyPackLink = {
  stx: 0,                  // Start of packet byte
  sourceId: 0,             // Source ID
  destinationId: 0,        // Destination ID
  len: 0,                  // Length of payload
  messageId: 0,            // Message ID
  code: 0,                 // Version code
  timestamp: 0,            // Timestamp
  componentId: 0,          // Component ID
  payload: new Array(35),  // Payload data (maximum of 35 bytes to accommodate the component ID)
  crc16: 0                 // CRC16 checksum
};</pre>
**example:**
[Decode Packet]https://github.com/homecircuit/tinypacklink/blob/aa1ab70a41012a0b293857f8b33939da93e7167f/examples/NodeJs/lib/tinypackdecode.js 

