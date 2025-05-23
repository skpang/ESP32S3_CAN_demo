/*
ESP32S3 CAN-bus demo
skpang.co.uk

For use with this board:
https://www.skpang.co.uk/products/esp32s3-can-bus-board

*/

#include <ESP32-TWAI-CAN.hpp>

#define CAN_TX   10  // Connects to CTX
#define CAN_RX   11  // Connects to CRX

#define ON  LOW
#define OFF HIGH

int LED_R = 39;
int LED_B = 40;
int LED_G = 38;

uint8_t count = 0;
CanFrame rxFrame; // Create frame to read 

void setup() {
    pinMode(LED_R,OUTPUT);
    pinMode(LED_G,OUTPUT);
    pinMode(LED_B,OUTPUT);
    digitalWrite(LED_B, OFF);
    digitalWrite(LED_G, OFF);
 
    digitalWrite(LED_R, ON);
    delay(200); 
    digitalWrite(LED_R, OFF);
    
    digitalWrite(LED_G, ON);
    delay(200); 
    digitalWrite(LED_G, OFF);
    
    digitalWrite(LED_B, ON);
    delay(200); 
    digitalWrite(LED_B, OFF);

    psramInit();
    // Set up serial for debugging
    Serial.begin(115200);
    delay(500);
    Serial.println("ESP32S3 CAN-Bus demo skpang.co.uk 05/2025");
    Serial.println((String)"Memory available in PSRAM : " +ESP.getFreePsram());
    
    // Set the pins
    ESP32Can.setPins(CAN_TX, CAN_RX);
 
    // Start the CAN bus at 500 kbps
    if(ESP32Can.begin(ESP32Can.convertSpeed(500))) {
        Serial.println("CAN bus started!");
    } else {
        Serial.println("CAN bus failed!");
    }
}

void loop() {
  static uint32_t lastStamp = 0;
  uint32_t currentStamp = millis();

  if(currentStamp - lastStamp > 400) {   
        lastStamp = currentStamp;
        canSender();  // call function to send data through CAN
    }
  
  canReceiver(); // call function to recieve data through CAN
}

void canSender() {
  digitalWrite(LED_B, ON);
     
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  // Serial.print("Sending packet ... ");
  CanFrame testFrame = { 0 };
  testFrame.identifier = 0x12;  // Sets the ID
  testFrame.extd = 0; // Set extended frame to false
  testFrame.data_length_code = 8; // Set length of data - change depending on data sent
  testFrame.data[0] = '1'; // Write data to buffer. data is not sent until writeFrame() is called.
  testFrame.data[1] = '2';
  testFrame.data[2] = '3';
  testFrame.data[3] = '4';
  testFrame.data[4] = '5';
  testFrame.data[5] = '6';
  testFrame.data[6] = '7';
  testFrame.data[7] = count++;

  ESP32Can.writeFrame(testFrame); // transmit frame
  // Serial.println("done");
  delay(10);
  digitalWrite(LED_B, OFF);
        
}

void canReceiver() {
  // try to parse packet
  if(ESP32Can.readFrame(rxFrame, 0)) { // 0 is the timeout value
    // Communicate that a packet was recieved
    Serial.printf("Received frame ID: %03X Len: %X  Data:", rxFrame.identifier,rxFrame.data_length_code);

    // Communicate packet information
    for(int i = 0; i <= rxFrame.data_length_code - 1; i ++) {
      Serial.printf(" %02X", rxFrame.data[i]); // 
    }
    Serial.println(" ");
  }
}