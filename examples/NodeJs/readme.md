
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
