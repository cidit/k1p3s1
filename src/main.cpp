#include <Arduino.h>
#include <LibRobus.h>
#include <stack>
#include "fonctions.h"
#include "parcours.h"

bool started_robot = true;
std::stack<int> retour;

int pose_x = 1;
int pose_y = 1;
bool bool_x[3] = {0, 0, 0};


void setup() {
  pinMode(pin_vert, INPUT);
  pinMode(pin_rouge, INPUT);
  pinMode(pin_ambiant, INPUT);
  pinMode(pin_5khz, INPUT);
  Serial.begin(9600);
  BoardInit();
  retour.push(pose_x);
  delay(2000);
}

void loop() {
  if (!started_robot && is_sifflet(pin_ambiant, pin_5khz, 5, 0, 100)) {
    started_robot = true;
    Serial.println("J'ai entendu le sifflet");
  } 
  else if (!started_robot) {
    Serial.println("Je n'ai pas entendu le sifflet");
  }

  if (started_robot) {
    try_parcours();
}  
}

