#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <stack>

std::stack<int> retour;

void setup(){
  retour.push(1); // Position initial
  pinMode(pin_vert, INPUT);
  pinMode(pin_rouge, INPUT);
  Serial.begin(9600);
  BoardInit();
  delay(2000);
}
int pose_x = 1;
int pose_y = 1;
bool bool_x[3] = {0,0,0};


void retourne(std::stack<int> retour, int pose_x){
  while (!retour.empty()){ 
    int top = retour.top();
    retour.pop();

    if (pose_x == top ){
      forwardInMeters(-0.5);
    }

    else if(pose_x<top ){
      turn_left();
      for (int i =0; i< top  - pose_x; i++){
        forwardInMeters(-0.5);
        pose_x ++;
      }
      turn_right();
      forwardInMeters(-0.5);
    }

    else{
      turn_right();
      for (int i =0; i< pose_x - top; i++){
        forwardInMeters(-0.5);
        pose_x --;
      }
      turn_left();
      forwardInMeters(-0.5);
    }
  }
}

void loop(){
  if(pose_y >= 11){ // FIN !
    retourne(retour, pose_x);
    exit(0);
  }
  
  
  if(check_open()){
    forwardInMeters(0.5);
    pose_y++;
    reset_bool(bool_x);
    retour.push(pose_x);
  }
  
  else if(pose_x < 2 and bool_x[pose_x+1] == false){
    bool_x[pose_x] = true;
    turn_right();
    Serial.println("0");
    if(check_open()){
      forwardInMeters(0.5);
      pose_x++;
      
    }
    else{
      bool_x[pose_x] = true;
      forwardInMeters(-0.5);
      pose_x--;
    }
    turn_left();
  }

  else if (pose_x == 2 and bool_x[pose_x-1] == true){ //
    pose_x-=2;
    turn_left();
    delay(100);
    forwardInMeters(0.5);
    forwardInMeters(0.5);
    Serial.println("1");
    turn_right();
  }

  else{
    bool_x[pose_x] = true;
    turn_left();
    forwardInMeters(0.5);
    pose_x--;
    Serial.println("2");
    turn_right();
  }
}

