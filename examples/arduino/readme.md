<strong><h2><p><center>Timestamp parameter for access Tinypacklink protocol</center></p></h2></strong>
<pre>struct Timestamp {
  uint8_t year;    // 0-99 (representing 2000-2099)
  uint8_t month;   // 1-12
  uint8_t day;     // 1-31
  uint8_t hour;    // 0-23
  uint8_t minute;  // 0-59
  uint8_t second;  // 0-59
};</pre>

**Example usage:**<br>
const timestamp = new Timestamp(21, 7, 7, 12, 30, 45); // Represents 2000-07-07 12:30:45<br>
console.log(timestamp);

<strong><h3>Encode/Decode Function</h3></strong>
<pre>uint32_t packTimestamp(const Timestamp& timestamp) {
  uint32_t packedTimestamp = 0;

  packedTimestamp |= (uint32_t)timestamp.year << 26;     // 6 bits for year
  packedTimestamp |= (uint32_t)timestamp.month << 22;    // 4 bits for month
  packedTimestamp |= (uint32_t)timestamp.day << 17;      // 5 bits for day
  packedTimestamp |= (uint32_t)timestamp.hour << 12;     // 5 bits for hour
  packedTimestamp |= (uint32_t)timestamp.minute << 6;    // 6 bits for minute
  packedTimestamp |= (uint32_t)timestamp.second;         // 6 bits for second

  return packedTimestamp;
}</pre>

<pre>void unpackTimestamp(uint32_t packedTimestamp, Timestamp& timestamp) {
  timestamp.year = (packedTimestamp >> 26) & 0x3F;      // Extract 6 bits for year
  timestamp.month = (packedTimestamp >> 22) & 0x0F;     // Extract 4 bits for month
  timestamp.day = (packedTimestamp >> 17) & 0x1F;       // Extract 5 bits for day
  timestamp.hour = (packedTimestamp >> 12) & 0x1F;      // Extract 5 bits for hour
  timestamp.minute = (packedTimestamp >> 6) & 0x3F;     // Extract 6 bits for minute
  timestamp.second = packedTimestamp & 0x3F;            // Extract 6 bits for second
}</pre>

<pre>// Function to convert hexadecimal string to byte array
void hexStringToByteArray(const String& hexString, uint8_t* byteArray, uint8_t byteArraySize) {
    size_t len = hexString.length();
    for (size_t i = 0; i < byteArraySize && i * 2 < len; ++i) {
        char hex[3];
        hex[0] = hexString[i * 2];
        hex[1] = hexString[i * 2 + 1];
        hex[2] = '\0';
        byteArray[i] = strtoul(hex, nullptr, 16);
    }
}</pre>

## Test 
<pre>
  uint8_t buff[2][64];
  uint8_t strlen1,strlen2;
  String mesg_str1 ="FFFE03E703E7270F0063018F9B00FFFE0BB7176F07CF03E701F3003100";
  String mesg_str2 ="FFFF03E803E827100064018F9C01FFFF0BB8177007D003E801F4003201";
  strlen1=mesg_str1.length()/2;
  strlen2=mesg_str2.length()/2;

  Serial.println("string length[1]:"+String(strlen1));
  Serial.println("string length[2]:"+String(strlen2));
  hexStringToByteArray(teststr1,buff[0],mesg_str1.length());
  for(int i = 0;i< strlen1;i++){
    Serial.print(" 0x");
    Serial.print(buff[0][i],HEX);
  }
  Serial.println();

  hexStringToByteArray(teststr2,buff[1],mesg_str2.length());
  for(int i = 0;i< strlen2;i++){
    Serial.print(" 0x");
    Serial.print(buff[1][i],HEX);
  }
  Serial.println();
</pre>
**output**
<pre>
string length[1]:29
string length[2]:29
 0xFF 0xFE 0x3 0xE7 0x3 0xE7 0x27 0xF 0x0 0x63 0x1 0x8F 0x9B 0x0 0xFF 0xFE 0xB 0xB7 0x17 0x6F 0x7 0xCF 0x3 0xE7 0x1 0xF3 0x0 0x31 0x0
 0xFF 0xFF 0x3 0xE8 0x3 0xE8 0x27 0x10 0x0 0x64 0x1 0x8F 0x9C 0x1 0xFF 0xFF 0xB 0xB8 0x17 0x70 0x7 0xD0 0x3 0xE8 0x1 0xF4 0x0 0x32 0x1  
</pre>
