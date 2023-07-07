<strong><h2><p><center>Tinypacklink protocol</center></p></h2></strong>
<pre>struct TinyPackLink {
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
};</pre>
**example:**
[Decode Packet]https://github.com/homecircuit/tinypacklink/blob/93dfec55f1c76b854c9ea8c46ebc38acf784bc9d/examples/arduino/libraries/Tinypacklink/examples/sample_decode/sample_decode.ino "The best search engine for privacy"

<strong><h2><p><center>Timestamp parameter for access Tinypacklink protocol</center></p></h2></strong>
<pre>classTimestamp{
constructor(year, month, day, hour, minute, second) {
    this.year = year;       // 0-99 (representing 2000-2099)
    this.month = month;     // 1-12
    this.day = day;         // 1-31
    this.hour = hour;       // 0-23
    this.minute = minute;   // 0-59
    this.second = second;   // 0-59
  }
}</pre>

**Example usage:**<br>
const timestamp = new Timestamp(21, 7, 7, 12, 30, 45); // Represents 2000-07-07 12:30:45<br>
console.log(timestamp);

<strong><h3>Encode/Decode Function</h3></strong>
<pre>function packTimestamp(timestamp) {
  let packedTimestamp = 0;
  packedTimestamp |= timestamp.year << 26;     // 6 bits for year
  packedTimestamp |= timestamp.month << 22;    // 4 bits for month
  packedTimestamp |= timestamp.day << 17;      // 5 bits for day
  packedTimestamp |= timestamp.hour << 12;     // 5 bits for hour
  packedTimestamp |= timestamp.minute << 6;    // 6 bits for minute
  packedTimestamp |= timestamp.second;         // 6 bits for second
  return packedTimestamp;
}</pre>

<pre>function unpackTimestamp(packedTimestamp, timestamp) {
  timestamp.year = (packedTimestamp >> 26) & 0x3F;      // Extract 6 bits for year
  timestamp.month = (packedTimestamp >> 22) & 0x0F;     // Extract 4 bits for month
  timestamp.day = (packedTimestamp >> 17) & 0x1F;       // Extract 5 bits for day
  timestamp.hour = (packedTimestamp >> 12) & 0x1F;      // Extract 5 bits for hour
  timestamp.minute = (packedTimestamp >> 6) & 0x3F;     // Extract 6 bits for minute
  timestamp.second = packedTimestamp & 0x3F;            // Extract 6 bits for second
}</pre>

<pre>function hexStringToByteArray(hexString, byteArray, byteArraySize) {
  const len = hexString.length;
  for (let i = 0; i < byteArraySize && i * 2 < len; ++i) {
    const hex = hexString.substr(i * 2, 2);
    byteArray[i] = parseInt(hex, 16);
  }
}</pre>
**Test**
<pre>
const buff = [[], []];
let strlen1, strlen2;
const mesg_str1 = "FFFE03E703E7270F0063018F9B00FFFE0BB7176F07CF03E701F3003100";
const mesg_str2 = "FFFF03E803E827100064018F9C01FFFF0BB8177007D003E801F4003201";
strlen1 = mesg_str1.length / 2;
strlen2 = mesg_str2.length / 2;

console.log("string length[1]: " + strlen1);
console.log("string length[2]: " + strlen2);

hexStringToByteArray(mesg_str1, buff[0], mesg_str1.length);
for (let i = 0; i < strlen1; i++) {
  console.log(" 0x" + buff[0][i].toString(16));
}

hexStringToByteArray(mesg_str2, buff[1], mesg_str2.length);
for (let i = 0; i < strlen2; i++) {
  console.log(" 0x" + buff[1][i].toString(16));
}    
</pre>
