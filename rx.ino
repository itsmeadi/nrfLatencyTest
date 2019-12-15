#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10,9); // CE, CSN

const byte address[6] = "00001";
void setup() {
  Serial.begin(250000);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  pinMode(LED_BUILTIN,OUTPUT);
}
unsigned long time_now = 0;


void loop() {
time_now = millis();  

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    delay(500);
      radio.stopListening();
      radio.openWritingPipe(address);
      radio.write(&text, sizeof(text));

      radio.openReadingPipe(0, address);
      radio.startListening();
    
  } 
}
