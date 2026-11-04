#include <Arduino.h>
#include <LibRobus.h>
#include <fonctions.h>
#include <utils/misc.h>

const int pin_detect_proxim_av = A0;
const int cote = A5;

int attack_angle = 0;
int bowling_states = 0;
int wall_states = 0;
int main_state = 2; // idle = 0, bowling = 1, wall = 2
float speed = -0.5;

void setup() {
    Serial.begin(9600);
    delay(1000);

    Serial.println("BOOTING MEGA");

    // BoardInit();

    pinMode(pin_detect_proxim_av, INPUT);
    pinMode(cote, INPUT);

    Serial.println("SETUP COMPLETE");
}

float distance(int pin_index){
  int value = analogRead(pin_index);

  if (value <= 0) {
    return -1;
  }
  float voltage = value * (5.0 / 1023.0);
  return 27.86 * pow(voltage, -1.15);
}

bool is_distance_greater_than(int x, int pin_index){
  if (x != -1){
    return x <= distance(pin_index);
  }
  return true;
}

// FUNCTIONS FOR THE BOWLING PIN
// TO START, PLAY THE FUNCTION search_for_bowling_pin()

void find_line_backwards(){
  forwardcorrection(-speed);
  if (attack_angle > 270){
    // Function to turn RIGHT until the line is found again;
  }
  else if (attack_angle > 180){
    // Function to turn RIGHT until the line is found again;
  }
  else if (attack_angle > 90){
    // Function to turn LEFT until the line is found again;
  }
  else{
    // Function to turn LEFT until the line is found again;
  }
}
void finish_attack_bowling_pin(){
  forwardInMetersTrapeze(-0.15);
  bowling_states = 3;
  
}
void attack_bowling_pin(){
  forwardcorrection(speed);
  if (!is_distance_greater_than(15, pin_detect_proxim_av)){
      bowling_states = 2;
    }
}
void search_for_bowling_pin(){
  // Function to rotate knowing the current angle;
  if (is_distance_greater_than(130, pin_detect_proxim_av)){
    // record angle via attack_angle;
      bowling_states = 1;
  }
  return;
}

// FUNCTIONS FOR THE WALL
// TO START, PLAY THE FUNCTION find_the_wall()

void find_the_wall(){
  forwardcorrection(speed);
  Serial.println(distance(pin_detect_proxim_av));
  if (!is_distance_greater_than(15, pin_detect_proxim_av)){
      turn_left(); // Function to go 90 degrees to the left ;
      wall_states = 1;
      Serial.println(wall_states);
      Serial.println("Changed to clear to the wall");
    }
}

void clear_the_wall(){
  forwardcorrection(speed);
  Serial.println(distance(cote));
  if (is_distance_greater_than(150, cote)){
      forwardInMetersTrapeze(-0.15);
      turn_right(); // Function to go 90 degrees to the right ;
      wall_states = 2;
      Serial.println(wall_states);
    }
}

void through_the_wall(){
  forwardInMetersTrapeze(-0.15); // Temporary, needs to pass the wall, so the distance is determined experimentaly (not 0.15)
  turn_right();
  wall_states = 3;
}
void back_to_line(){
  forwardcorrection(speed);
  // if see line
  forwardInMetersTrapeze(-0.15);
  turn_left();
}

void loop() {
  switch (main_state){
    case 0:
      Serial.print("Idle");
      break;
    case 1:
      switch (bowling_states)
      {
        case 0:
        search_for_bowling_pin();
        break;
        case 1:
        attack_bowling_pin();
        break;
        case 2:
        finish_attack_bowling_pin();
        break;
        case 3:
        find_line_backwards();
        break;
      }
      break;
    case 2:
      switch (wall_states)
      {
        case 0:
        find_the_wall();
        break;
        case 1:
        clear_the_wall();
        break;
        case 2:
        through_the_wall();
        break;
        case 3:
        back_to_line();
        break;
      }
      break;
  }

}

