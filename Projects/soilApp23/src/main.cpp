
/*#define BLYNK_TEMPLATE_ID "TMPL2-iKUnllq"
#define BLYNK_TEMPLATE_NAME "SmartIrrigation"*/

#define BLYNK_TEMPLATE_ID "TMPL2HytvxiJS"
#define BLYNK_TEMPLATE_NAME "smart irrigation"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp32.h>
//#if CONFIG_FREERTOS_UNICORE
  //static const BaseType_t app_cpu = 0;
//#else
  //static const BaseType_t app_cpu = 1;
//#endif
#define BLYNK_PRINT Serial

/*#define BLYNK_AUTH_TOKEN "YhYXpHT3WbM2TO3cBpg1y910kJNyox-z"*/
#define BLYNK_AUTH_TOKEN "_6FAqw9YXJO36uaRId0kHj-1X8nSLMwB"

#define WATER_PUMP 5
#define SOIL_SENSOR 34
const char* ssid = "Haxor";
const char* pass = "wbbe2586";
/*const char* ssid = "es";
const char* pass = "mydomain123";*/
BlynkTimer timer;
// put function declarations here:
static const uint8_t queue_len = 7;
static QueueHandle_t queue1;




void soilSensor(void *parameters){
  pinMode(SOIL_SENSOR,INPUT);
  while(1){
    int soilReading = analogRead(SOIL_SENSOR);
   
    Serial.println("  - Adding  to queue");
    xQueueSend(queue1,&soilReading,portMAX_DELAY);
    int percent = map(soilReading,0,4095,0,100);
    Blynk.virtualWrite(V1, percent);
    vTaskDelay(3000);
  }
  
}

void soilSensor2(){
  pinMode(SOIL_SENSOR,INPUT);

    int soilReading = analogRead(SOIL_SENSOR);
   
    Serial.println("  - Adding  to queue");
    //xQueueSend(queue1,&soilReading,portMAX_DELAY);
    //client.publish("esp32/humidity",(char *)soilReading);
    Blynk.virtualWrite(V1, soilReading);
    //delay(300);
    //vTaskDelay(500 / portTICK_PERIOD_MS);
  
  
}

void irrigateSoil(void *parameters){
  while(1){
    int soilReading;
   
    Serial.println("  - Reading from queue");
    xQueueReceive(queue1,&soilReading,portMAX_DELAY);
    if (soilReading < 2000)
    {
      /* code */
      digitalWrite(WATER_PUMP,LOW);
       Serial.println("  - value is");
        Serial.println(soilReading);
    }
    else
    {
      digitalWrite(WATER_PUMP,HIGH);
      Serial.println("  - value is when low");
      Serial.println(soilReading);
    }
    
    delay(300);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  //vTaskDelay(1000 / portTICK_PERIOD_MS);
  pinMode(WATER_PUMP,OUTPUT);
  timer.setInterval(2000L, soilSensor2);
  
  // CREATING QUEUE
  queue1 = xQueueCreate(queue_len,sizeof(int));
  //TASK1
  xTaskCreatePinnedToCore(soilSensor,
              "Take moisture reading",
              8000,
              NULL,
              1,
              NULL,
              1
  );

  //TASK2
  xTaskCreatePinnedToCore(irrigateSoil,
              "Turn on water pump",
              1024,
              NULL,
              1,
              NULL,
              1
  );

  //TASK3
  

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run(); 
  vTaskDelete (NULL);
}

// put function definitions here:
