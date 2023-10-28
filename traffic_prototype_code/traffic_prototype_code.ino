
#include <SPI.h>
#include <MFRC522.h>
#include <Thread.h>
#include <ThreadController.h>


#define RED_LIGHT D12
#define GREEN_LIGHT D11


int countPins[5] = {D2,D3,D4,D5,D9};

unsigned long previousTime = millis();
unsigned long countSecondsPrevious = millis();
int count =10;

bool red_light_status = LOW;
bool green_light_status = HIGH;


void setup() { 
  Serial.begin(115200);

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(GREEN_LIGHT, OUTPUT);
  for (int i=0; i<5; i++){
    pinMode(countPins[i], OUTPUT);
  }
}


void loop() {
  unsigned long currentTime = millis();

  // 빨간불에서 -> 초록불
  if (red_light_status == HIGH && count == 0){
    Serial.println("red Off");
    red_light_status = LOW;
    green_light_status = HIGH;
    digitalWrite(RED_LIGHT, red_light_status);
    digitalWrite(GREEN_LIGHT, green_light_status);
    for (int i=0; i<5; i++){
      digitalWrite(countPins[i], green_light_status);
    }
    previousTime = currentTime;
    countSecondsPrevious = currentTime;
    count=10;
  }

  // 초록불에서 -> 빨간불
  if (red_light_status == LOW && count == 0){
    Serial.println("green Off");
    red_light_status = HIGH;
    green_light_status = LOW;
    digitalWrite(RED_LIGHT, red_light_status);
    digitalWrite(GREEN_LIGHT, green_light_status);
    for (int i=0; i<5; i++){
      digitalWrite(countPins[i], green_light_status);
    }
    previousTime = currentTime;
    count = 5;
  }

  // 초록불일 때 5초부터 count
  if (currentTime - countSecondsPrevious >= 1000){
    count = count - 1;
    Serial.println(count);
    countSecondsPrevious = currentTime;
    digitalWrite(countPins[count-1], LOW);
    Serial.println(countPins[count-1] + "LOW");
    
  }
  
}


