#include <Arduino.h>
#include "model.h"
#include <ArduTFLite.h>

constexpr int tensorArenaSize = 8 * 1024;
alignas(16) byte tensorArena[tensorArenaSize];

const int adc0 = 26;
const int adc1 = 27;
const int led_blue = 22;
int v1,v2;

float mean[2] = {260.43,138.62 };
float std2[2] = {250.20, 112.884};

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico!");
  analogReadResolution(10);

  pinMode(led_blue, OUTPUT);
  digitalWrite(led_blue, LOW);

  if (!modelInit(model, tensorArena, tensorArenaSize)){
    Serial1.println("Model initialization failed!");
    while(true);
  }
  Serial1.println("Model initialization done.");
}

void loop() {
 v1 = analogRead(adc0);
 v2 = analogRead(adc1);

 float v1_scl = (v1-mean[0])/std2[0];
 float v2_scl = (v2-mean[1])/std2[1];
 //Serial1.print(v1);
 //Serial1.print(",");
 //Serial1.println(v2);
modelSetInput(v1_scl,0);
modelSetInput(v2_scl,1);
if(!modelRunInference()){
    Serial1.println("RunInference Failed!");
    return;
}
float result = modelGetOutput(0);
Serial1.println(result);

if(result>0.5){
  digitalWrite(led_blue, HIGH);
}else{
  digitalWrite(led_blue, LOW);
}

/*
if(v1>512 && v2>750){
  digitalWrite(led_blue, HIGH);
}else{
  digitalWrite(led_blue, LOW);
}*/

 delay(500);
}
