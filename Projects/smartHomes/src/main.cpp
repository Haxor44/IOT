#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
// put function declarations here:
#define relay 2

const char* ssid = ""
const char* pass = ""
const char* broker = "192.168.129.252";
const char* topic = "light";
const char* topic2 = "test";

WiFiClient esp;
PubSubClient client(esp);


void setupWifi(){
  delay(100);
  Serial.println("Connecting");

  WiFi.begin(ssid,pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    /* code */
    delay(100);
    Serial.println("-");
  }
  Serial.println("Connected to:");
  Serial.println(ssid);
  
}

void reconnect(){
  while (!client.connected())
  {
    /* code */
    Serial.println("Connected to:");
    Serial.println(broker);
    if (client.connect("smart","",""))
    {
      /* code */
      Serial.println("Connected to:");
      Serial.println(broker);
      client.subscribe(topic2);
    }
    else {
      Serial.println("Trying to connect!!!");
      delay(5000);
    }
    
  }
  
}

void received(char* topic, byte* message, unsigned int length){
  Serial.print("Received message!!!");
  
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 setupWifi();
 client.setServer(broker,1883);
 client.setCallback(received);
 //pinMode(relay,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   /*digitalWrite(relay,LOW);
    delay(500);
    digitalWrite(relay,HIGH);
    delay(500);*/

    if (!client.connected())
    {
      /* code */
      reconnect();
    }
    client.loop();
    
}

// put function definitions here:
