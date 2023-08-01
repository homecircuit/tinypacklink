function calculateCRC16(data, length) {
    let crc = 0xFFFF;
  
    for (let i = 0; i < length; i++) {
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

// Helper function to convert an array to a hexadecimal string
function arrayToHexString(arr) {
    return arr.map(num => num.toString(16).padStart(2, '0')).join('');
}

function packget(sourceId, destinationId, messageId, version, componentId, data) {
    const stx = 55; // Start of Text in hexadecimal

    // address data
    sourceId = (sourceId).toString(16).padStart(4, '0');
    destinationId = (destinationId).toString(16).padStart(4, '0');
    messageId = (messageId).toString(16).padStart(2, '0'); // Fix here - pad to 2 characters
    version = (version).toString(16).padStart(2, '0');
    timestamp = (parseInt(Date.now() / 1000)).toString(16); // Correct timestamp calculation
    componentId = (componentId).toString(16).padStart(2, '0');

    // len data
    datalen = messageId + version + (timestamp).toString(16).padStart(8, '0') + componentId+arrayToHexString(data);
    
    var messageArray = datalen.match(/.{2}/g).map(hex => parseInt(hex, 16));
    var messageLength = messageArray.length;
    len = (messageLength).toString(16).padStart(2, '0');

    // cal data crc16
    data_crc16 = stx + sourceId + destinationId + len + messageId + version + timestamp + componentId + arrayToHexString(data);
    var crcArray = data_crc16.match(/.{2}/g).map(hex => parseInt(hex, 16));
    var crcLength = crcArray.length;
    console.log((crcLength));
    crc16 = calculateCRC16(crcArray, crcLength).toString(16).padStart(4, '0'); // Fix here - pad to 4 characters

    // package msg    
    packget_data = stx + sourceId + destinationId + len + messageId + version + timestamp + componentId + arrayToHexString(data) + crc16;
    console.log(packget_data);
}

// Given values
const sourceId = 1;
const destinationId = 34952;
const messageId = 3; 
const version = 1;
const componentId = 1;
const data = [2, 0];

// Call the packget function
packget(sourceId, destinationId, messageId, version, componentId, data);
