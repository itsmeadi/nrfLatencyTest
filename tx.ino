#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(10,9); // CE, CSN
const byte address[6] = "00001";
void(* resetFunc) (void) = 0;
void setup() {

//  radio.setRetries(2, 8);
  //radio.setAutoAck(true);

  Serial.begin(250000);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);

  radio.stopListening();
}
void loop() {
  unsigned long timeNow=millis();
  
  char text[] = "Hello World";
  sprintf(text,"%u",timeNow);
 // Serial.print("Send=");
  //Serial.println(text);

  radio.write(&text, sizeof(text));

  radio.openReadingPipe(0, address);
  radio.startListening();

  int retry=0;
  while(!radio.available()) 
  {
    retry++;
    if (retry>100){
      Serial.println("Resetting");
      resetFunc();
    }
    delay(10);
  }
    char textRec[32] = "";
    radio.read(&textRec, sizeof(textRec));
//    Serial.print("Received ack=");
//    Serial.println(textRec);
    unsigned long timeRecLong;
    timeRecLong=atol(textRec);
    if (timeRecLong==timeNow){
      Serial.print("Diff=");
      Serial.println(millis()-timeRecLong);
    } else{
      Serial.println("OLD");
      radio.read(&textRec, sizeof(textRec));
      radio.read(&textRec, sizeof(textRec));
      radio.read(&textRec, sizeof(textRec));
      radio.read(&textRec, sizeof(textRec));
    }
    
    radio.stopListening();
    radio.openWritingPipe(address);
  
}
