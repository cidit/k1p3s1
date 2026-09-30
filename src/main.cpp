#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <stack>

bool matrice_de_mur[10][3]; // 0 = il n'y a pas de mur, 1 = il y a un mur, 

String etat_directionnel_du_robot = "Bas"; // Etat directionnel du robot, a nommer avec un string : Bas, Gauche, Droite, Haut
std::vector<int> position_robot = {1,0}; // position x,y

void setup(){
  
  pinMode(pin_vert, INPUT);
  pinMode(pin_rouge, INPUT);
  Serial.begin(9600);
  BoardInit();
  delay(2000);
}

void nouvelle_position(){
  if (etat_directionnel_du_robot == "Droite"){
    position_robot[0] = position_robot[0] + 1;
  }
  else if (etat_directionnel_du_robot == "Gauche"){
    position_robot[0] = position_robot[0] - 1;
  }
  else if (etat_directionnel_du_robot == "Haut"){
    position_robot[1] = position_robot[1] - 1;
  }
  else{position_robot[1] = position_robot[1] + 1;
  }
}

bool mur_en_avant_check(){
  if(check_open()==true){
    return true;
  }
  else if(check_open()==false){
    return false;
  }
}

void tourner_vers_le_bas(){
  if (etat_directionnel_du_robot == "Droite"){
    turn_right();
  }
  else if (etat_directionnel_du_robot == "Gauche"){
    turn_left();
  }
}

void nouvel_endroit_check(){
  for (int i = 0; i < 3; ++i){
    if (matrice_de_mur[position_robot[1]][i] == 0){
      if (i > position_robot[0]){ // la nouvelle position est a la droite du robot selon la matrice
        turn_left();
        etat_directionnel_du_robot = "Droite";
        delay(300);
      }
      else{ // la nouvelle position est a la gauche du robot selon la matrice
        turn_right();
        etat_directionnel_du_robot = "Gauche";
        delay(300);
      }
      Serial.print("Direction : ");
      Serial.println(etat_directionnel_du_robot);
      if (abs(i-position_robot[0]) > 1){
        delay(300);
        avancer();
        nouvelle_position();
        avancer();
        nouvelle_position();
        tourner_vers_le_bas();
      }
      else{
        delay(300);
        avancer();
        nouvelle_position();
        tourner_vers_le_bas();
      }
      Serial.print("x = ");
      Serial.print(position_robot[0]);
      Serial.print(", y = ");
      Serial.println(position_robot[1]);
      return;
    }
  }
}


void loop(){
  if (!mur_en_avant_check()){
    matrice_de_mur[position_robot[1]][position_robot[0]] = 1;
    nouvel_endroit_check();
  }
  else{
    avancer();
    nouvelle_position();
  }
}