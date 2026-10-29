#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

bool started_robot = true;
std::stack<int> retour;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  pinMode(pin_vert, INPUT);
  pinMode(pin_rouge, INPUT);
  pinMode(pin_ambiant, INPUT);
  pinMode(pin_5khz, INPUT);
  Serial.begin(9600);
  BoardInit();
  retour.push(pose_x);
  Serial.println("Color View Test!");
  if (tcs.begin()) {
      Serial.println("Found sensor");
  } else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
  }
  delay(2000);
}
int range = 20;
void loop() {
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED

  delay(1000);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED

  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.println(blue);

  if (abs(red - 222) < range and abs(green - 305) < range and abs(blue - 242) < range){
    Serial.println("VERT");
  } else if(abs(red - 160) < range and abs(green - 260) < range and abs(blue - 245) < range){
    Serial.println("BLEU");
  } else if(abs(red - 310) < range and abs(green - 350) < range and abs(blue - 200) < range){
    Serial.println("JAUNE");
  } else if(abs(red - 310) < range and abs(green - 275) < range and abs(blue - 180) < range){
    Serial.println("ORANGE");
  } else if(abs(red - 376) < range and abs(green - 300) < range and abs(blue - 250) < range){
    Serial.println("ROUGE");
  }
}