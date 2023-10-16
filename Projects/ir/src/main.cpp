#include <Arduino.h>
#include <IRremote.h>

#define RECV_PIN 34
IRrecv receiver(RECV_PIN);
decode_results results;
// put function declarations here:




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  receiver.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(receiver.decode(&results)){
    Serial.println(results.value,HEX);
    receiver.resume();
  }
}


// put function definitions here:
