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
bool force_back = false;


void retourne(std::stack<int> retour, int pose_x){
  while (!retour.empty()){ 
    int top = retour.top();
    Serial.println(top);
    retour.pop();

    if (pose_x == top ){
      forwardInMetersTrapeze(-0.5);
    }

    else if(pose_x<top ){
      turn_left();
      delay(300);
      for (int i =0; i <= top  - pose_x; i++){
        forwardInMetersTrapeze(-0.5);
        pose_x ++;
      }
      turn_right();
      delay(300);
      forwardInMetersTrapeze(-0.5);
    }

    else{
      turn_right();
      delay(300);
      for (int i =0; i <= pose_x - top; i++){
        forwardInMetersTrapeze(-0.5);
        pose_x --;
      }
      turn_left();
      delay(300);
      forwardInMetersTrapeze(-0.5);
    }
  }
}

void loop(){
  if(pose_y >= 11){ // FIN !
    delay(2000);
    retourne(retour, pose_x);
    exit(0);
  }
  
  
  if(check_open() and !force_back){
    forwardInMeters(0.5);
    pose_y++;
    delay(300);
    if(!check_open() and pose_y % 2 == 0){
      forwardInMeters(-0.5);
      pose_y--;
      bool_x[pose_x] = true;\
      force_back = true;
    }else{
      reset_bool(bool_x);
      retour.push(pose_x);
    }
    
  }
  
  else if(pose_x < 2 and bool_x[pose_x+1] == false){
    bool_x[pose_x] = true;
    force_back = false;
    turn_right();
    delay(300);
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
    delay(300);
  }

  else if (pose_x == 2 and bool_x[pose_x-1] == true){ 
    force_back = false;
    pose_x-=2;
    turn_left();
    delay(300);
    forwardInMeters(0.5);
    forwardInMeters(0.5);
    Serial.println("1");
    turn_right();
    delay(300);
  }

  else{
    force_back = false;
    bool_x[pose_x] = true;
    turn_left();
    delay(300);
    forwardInMeters(0.5);
    pose_x--;
    Serial.println("2");
    turn_right();
    delay(300);
  }

}