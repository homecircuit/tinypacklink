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
[Decode Packet]https://github.com/homecircuit/tinypacklink/blob/aa1ab70a41012a0b293857f8b33939da93e7167f/examples/NodeJs/lib/tinypackdecode.js "The best search engine for privacy"


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
