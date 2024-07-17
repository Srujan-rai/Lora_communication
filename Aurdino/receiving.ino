#include <SoftwareSerial.h>

#define MD0_PIN 7
#define MD1_PIN 6

SoftwareSerial lora(2, 3); // RX---3
//TX--3

void setup() {
  pinMode(MD0_PIN, OUTPUT);
  pinMode(MD1_PIN, OUTPUT);
  

  digitalWrite(MD0_PIN, LOW);
  digitalWrite(MD1_PIN, LOW);

  Serial.begin(9600);
  lora.begin(9600);
}

void loop() {
  if (lora.available()) {
    String receivedMessage = lora.readString();
    Serial.print("Received message: ");
    Serial.println(receivedMessage);
  }
}
