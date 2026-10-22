#include <Arduino.h>
#include <LibRobus.h>
#include <stack>
#include "fonctions.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

bool started_robot = true;
std::stack<int> retour;

int pose_x = 1;
int pose_y = 1;
bool bool_x[3] = {0, 0, 0};
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

  if (abs(red - 222) < 30 and abs(green - 305) < 30 and abs(blue - 242) < 30){
    Serial.println("VERT");
  }

  
  /*
  if (!started_robot && is_sifflet(pin_ambiant, pin_5khz, 5, 0, 100)) {
    started_robot = true;
    Serial.println("J'ai entendu le sifflet");
  } 
  else if (!started_robot) {
    Serial.println("Je n'ai pas entendu le sifflet");
  }

  if (started_robot) {
    if ((check_open() and pose_x != 1) or (check_open() and pose_x == 1 and pose_y == 5))  {
      forwardInMetersTrapeze(1.0);
      pose_y += 2;
      //delay(300);
      reset_bool(bool_x);
      retour.push(pose_x);
  }

  else if (pose_x < 2 && bool_x[pose_x + 1] == false) {
    bool_x[pose_x] = true;
    turn_right();
    delay(300);
    if (check_open()) {
      forwardInMetersTrapeze(0.5);
      pose_x++;
    } else {
      bool_x[pose_x] = true;
      forwardInMetersTrapeze(-0.5);
      pose_x--;
    }
    
    turn_left();
    delay(300);
  } 

  else if (pose_x == 2 && bool_x[pose_x - 1] == true) { 
    pose_x -= 2;
    turn_left();
    delay(300);
    forwardInMetersTrapeze(1.0);
    turn_right();
    delay(300);
  } 
  else if (pose_x == 0 and bool_x[pose_x + 2] == false) {
    bool_x[pose_x] = true;
    pose_x += 2;
    turn_right();
    delay(300);
    forwardInMetersTrapeze(1.0);
    turn_left();
    delay(300);
  }
  else  {
    bool_x[pose_x] = true;
    turn_left();
    delay(300);
    forwardInMetersTrapeze(0.5);
    pose_x--;
    turn_right();
    delay(300);
  }
  if (pose_y == 11 and pose_x == 0){
    // tOURNE À droite, avance, tourne a droite moteur max
    turn_right();
    delay(300);
    forwardInMetersTrapeze(0.5);
    turn_right();
    delay(300);
    forwardInMetersTrapeze(5.5);
    exit(0);

  } 

  else if (pose_y == 11 and pose_x == 2){
      // Tourne a gauche, avance, tourne a gauche et moteur max
      turn_left();
      delay(300);
      forwardInMetersTrapeze(0.5);
      turn_left();
      delay(300);
      forwardInMetersTrapeze(5.5);
      exit(0);

    } 
    if (pose_y == 5){
      bool_x[1] = false;
    }
}  */
}

