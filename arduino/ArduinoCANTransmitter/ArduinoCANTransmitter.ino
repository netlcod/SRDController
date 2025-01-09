// v 1.1
#include <SPI.h>
#include <mcp2515.h>

uint8_t stx = 0xAA;
uint8_t etx = 0xFF;

const int SPI_CS_PIN = 10;
MCP2515 mcp2515(SPI_CS_PIN);  

struct can_frame canFrame;
volatile MCP2515::ERROR status = MCP2515::ERROR::ERROR_NOMSG;

MCP2515::ERROR receiveCan(const MCP2515::RXBn rxBuffer, struct can_frame *frame) {
  MCP2515::ERROR result = mcp2515.readMessage(rxBuffer, frame);
}

bool readFromSerialCanFrame(struct can_frame *frame) {
  return (Serial.readBytes((char*)frame, sizeof(can_frame)) == sizeof(can_frame));
}

bool writeToSerialCanFrame(struct can_frame *frame) {
  size_t bytes = 0;
  bytes += Serial.write(stx);
  bytes += Serial.write(sizeof(uint32_t) + sizeof(uint8_t) + frame->can_dlc);
  bytes += Serial.write((char*)frame, sizeof(can_frame));
  bytes += Serial.write(etx);
  return bytes;
}

void printCanPacket(struct can_frame *frame) {
  if ((frame->can_id & 0x80000000) == 0x80000000) {
      Serial.print("Extendend RX: 0x");
      Serial.print(frame->can_id & 0x1FFFFFFF, HEX);
    } else {
      Serial.print("Standard RX: 0x");
      Serial.print(frame->can_id, HEX);
    }
    
    Serial.print(" "); 
    Serial.print(frame->can_dlc, DEC);
    Serial.print(" "); 
    for (int i = 0; i<frame->can_dlc; i++)  {
      Serial.print(frame->data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
}

void setup() {
  Serial.begin(115200);

  while (MCP2515::ERROR::ERROR_OK != mcp2515.reset()) {
    Serial.println("CAN init fail, retry...");
    delay(1000);
  }
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("Initialize sucessful");
}

void loop() {
  if (mcp2515.readMessage(&canFrame) == MCP2515::ERROR_OK) {
      writeToSerialCanFrame(&canFrame);
  }

  if (Serial.available() > 0) { 
    struct can_frame canSerialFrame;
    if (readFromSerialCanFrame(&canSerialFrame)) {
      mcp2515.sendMessage(&canSerialFrame);
    }     
  }
  
}
