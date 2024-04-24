//==================================================================================//

#include <CAN.h>
#define TX_GPIO_NUM 22  // Connects to CTX
#define RX_GPIO_NUM 23  // Connects to CRX
// Change one of these to 1 to load that corresponding Node code onto the board
#define NODE_A 1
#define NODE_B 0
#define NODE_C 0

int LED_active = 0;
static unsigned long lastReceivedTime = 0; // Stores the last time a message was received
//==================================================================================//

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(1000);

  if (NODE_A == 1) {
    pinMode(18, INPUT);
    Serial.println("Node A");
  }
  else if (NODE_B == 1){
     pinMode(2, OUTPUT);
     Serial.println("Node B");
  }
  else {
    Serial.println("Node C");
  }
  // Set the pins
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1)
      ;
  } else {
    Serial.println("CAN Initialized");
  }
}

//==================================================================================//

void loop() {
  if (NODE_A == 1){
    nodeA();
  }
  else if (NODE_B == 1){
    nodeB();
  }
  else if (NODE_C == 1){
    nodeC();
  }
}

//===================================== NODES =========================================//

// Node A listens to the button and sends messages over CAN
void nodeA() {
  // Serial.print("TEC: ");
  // Serial.println(CAN.readTEC());
    CAN.beginPacket(0x12);  //sets the ID and clears the transmit buffer
    // Oscilloscope: Sent 0b10110100
    // CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.write(0b10110100);
    CAN.endPacket();
  // === Uncommented for Oscilloscope
  // if (digitalRead(18) == HIGH) {
  //   // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  //   // Serial.print("Sending packet ... ");
  //   Serial.println("sending 1");
  //   CAN.beginPacket(0x12);  //sets the ID and clears the transmit buffer
  //   // CAN.beginExtendedPacket(0xabcdef);
  //   CAN.write('1');  //write data to buffer. data is not sent until endPacket() is called.
  //   CAN.write('1');
  //   CAN.write('1');
  //   CAN.write('1');
  //   CAN.write('1');
  //   CAN.write('1');
  //   CAN.write('1');
  //   CAN.write('1');
  //   CAN.endPacket();

  //   //RTR packet with a requested data length
  //   // CAN.beginPacket(0x12, 3, true);
  //   // CAN.endPacket();

  //   // Serial.println("done");
  // }
  // else{
  //   Serial.println("Sending 0");

  //   CAN.beginPacket(0x12);  //sets the ID and clears the transmit buffer
  //   CAN.write('0');  //write data to buffer. data is not sent until endPacket() is called.
  //   CAN.write('0');
  //   CAN.write('0');
  //   CAN.write('0');
  //   CAN.write('0');
  //   CAN.write('0');
  //   CAN.write('0');
  //   CAN.write('0');
  //   CAN.endPacket();

  //   //RTR packet with a requested data length
  //   // CAN.beginPacket(0x12, 3, true);
  //   // CAN.endPacket();

  //   // Serial.println("done");
  // }
  delay(0.5); // Pre-oscilloscope this was 500
}

// Node B listens for the message from Node A and turns on the LED
void nodeB() {
  const unsigned long timeoutDuration = 100;
  // try to parse packet
  int packetSize = CAN.parsePacket();
  
  if (packetSize) {
    lastReceivedTime = millis();
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }
    char readChar = (char)CAN.read();
    Serial.print("readChar: ");
    Serial.println(readChar);
    
    if (readChar == '1'){
      digitalWrite(2, HIGH);
    }
    else if (readChar == '0'){
      digitalWrite(2, LOW);
    }
    
    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);
    
    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      // while (CAN.available()) {
      //   Serial.print(" packet content: ");
      //   Serial.print((char)CAN.read());
      // }
      Serial.println();
    }

    Serial.println();
  }
    // if (millis() - lastReceivedTime > timeoutDuration) {
    //  digitalWrite(2, LOW); // Turn the LED off if timeout has expired
    // }   
  delay(10); // Pre-oscilloscope this was 0
}

// Node C penetrates
void nodeC() {
  
    // send packet: id is 11 bits, packet can contain up to 8 bytes of data
    // Serial.print("Sending packet ... ");
    Serial.println("sending 1");
    CAN.beginPacket(0x11);  // BUS OFF: Change to 0x12. sets the ID and clears the transmit buffer
    // CAN.beginExtendedPacket(0xabcdef);
    CAN.write('1');  // BUS OFF: change to 1 write data to buffer. data is not sent until endPacket() is called.
    CAN.write('1');
    CAN.write('1');
    CAN.write('1');
    CAN.write('1');
    CAN.write('1');
    CAN.write('1');
    CAN.write('1');
    CAN.endPacket();

  delay(10); // Pre-oscilloscope this was 100
  
}

//================================GENERIC SENDER/RECEIVER===========================================//

void canSender() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print("Sending packet ... ");

  CAN.beginPacket(0x12);  //sets the ID and clears the transmit buffer
  // CAN.beginExtendedPacket(0xabcdef);
  CAN.write('1');  //write data to buffer. data is not sent until endPacket() is called.
  CAN.write('2');
  CAN.write('3');
  CAN.write('4');
  CAN.write('5');
  CAN.write('6');
  CAN.write('7');
  CAN.write('8');
  CAN.endPacket();

  //RTR packet with a requested data length
  CAN.beginPacket(0x12, 3, true);
  CAN.endPacket();

  Serial.println("done");

  delay(1000);
}

void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print((char)CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}

//==================================================================================//
