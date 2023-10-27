
#include <SPI.h>
#include <MFRC522.h>
#include <Thread.h>
#include <ThreadController.h>

#define SS_1_PIN D10
#define SS_2_PIN D8
#define RST_PIN D9
#define NR_OF_READERS   2

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

#define RED_LIGHT D3
#define GREEN_LIGHT D4


MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];



void tagInfoGet() {
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
      Serial.println("===========================");
      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader

 
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

bool red_light_status = HIGH;
bool green_light_status = LOW;
unsigned long previousTime = millis();
void trafficControl(){
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= 10000){
    red_light_status = !red_light_status;
    green_light_status = !green_light_status;
    digitalWrite(RED_LIGHT, red_light_status);
    digitalWrite(GREEN_LIGHT, green_light_status);
    previousTime = currentTime;
  }
  
}

Thread threadTag;
Thread threadTraffic;

void setup() { 
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }



  pinMode(RED_LIGHT, OUTPUT);
  pinMode(GREEN_LIGHT, OUTPUT);

  threadTag.onRun(tagInfoGet);
  threadTraffic.onRun(trafficControl);

  threadTag.setInterval(20);
  threadTraffic.setInterval(20);

}


void loop() {
  digitalWrite(RED_LIGHT, red_light_status);
  digitalWrite(GREEN_LIGHT, green_light_status);
  threadTag.run();
  threadTraffic.run();
}


