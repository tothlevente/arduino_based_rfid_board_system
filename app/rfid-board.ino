#include "SPI.h"
#include "MFRC522.h"

int SsPin = 10;
int RstPin = 9;

int LedPinR = 2;
int LedPinG = 3;
int LedPinB = 4;

int alarmButton = 6;
int confirmationButton = 5;

int alarmButtonState = 0;
int confirmationButtonState = 0;

int relay = 7;
int relayState = 0;

int incoming = 0;

String id[5];
String strId = "";

bool ledBlinking = false;

MFRC522 rfid(SsPin, RstPin);
MFRC522::MIFARE_Key key;

  void setup() {
    
    Serial.begin(9600);
    SPI.begin();
    
    rfid.PCD_Init();
    
    pinMode(LedPinR, OUTPUT);
    pinMode(LedPinG, OUTPUT);
    pinMode(LedPinB, OUTPUT);
    
    pinMode(alarmButton, INPUT);
    pinMode(confirmationButton, INPUT);
    pinMode(relay, INPUT);
    
    // Set here your card's ID's:
    id[0] = "";
    id[1] = "";
    id[2] = "";
    id[3] = "";
    id[4] = "";
    
  }

  void loop() {
    
    relayState = digitalRead(relay);
    alarmButtonState = digitalRead(alarmButton);
    confirmationButtonState = digitalRead(confirmationButton);
    
    if (alarmButtonState == HIGH) {
      Serial.println("You pushed the alarm button!");
      digitalWrite(relay, LOW);
      ledBlinking = true;
    }
    
    if (ledBlinking) {
      
      digitalWrite(LedPinR, HIGH);
      delay(800);
      digitalWrite(LedPinR, LOW);
      delay(800);
      digitalWrite(LedPinG, HIGH);
      delay(800);
      digitalWrite(LedPinG, LOW);
      delay(800);
    }
    
    if (confirmationButtonState == HIGH) {
      
      Serial.println("You pushed the confirmation button!");
      digitalWrite(LedPinR, LOW);
      digitalWrite(LedPinG, LOW);
      digitalWrite(LedPinB, LOW);
      digitalWrite(relay, LOW);
      ledBlinking = false;
    }
    
    
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Your tag is not of type MIFARE Classic! :/ "));
      return;
    }
    
    strId = "";

    for (byte i = 0; i < 4; i++) {
      strId +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i!=3 ? ":" : "");
      
    }

    strId.toUpperCase();

    Serial.print("Tap card key: ");
    Serial.println(strId);

    if (strId == id[0] || strId == id[1] || strId == id[2] || strId == id[3] || strId == id[4]) {
      
      digitalWrite(LedPinG, HIGH);
      digitalWrite(relay, HIGH);
      delay(5000);
      digitalWrite(LedPinG, LOW);
      digitalWrite(relay, LOW);
      
    } else {
      digitalWrite(LedPinR, HIGH);
      delay(2000);
      digitalWrite(LedPinR, LOW);
      
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}
