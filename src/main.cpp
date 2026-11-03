#include <Arduino.h>
#include <LibRobus.h>
#include <fonctions.h>
const int pin_detect_proxim_av = A0;
const int pin_detect_proxim_cote = A1;
unsigned long previousTime = 0;
unsigned long counter = 0;
int attack_angle = 0;
int bowling_states = 0;
void setup() {
    Serial.begin(9600);
    delay(1000);

    Serial.println("BOOTING MEGA");

    BoardInit();

    pinMode(pin_detect_proxim_av, INPUT);
    pinMode(pin_detect_proxim_cote, INPUT);

    Serial.println("SETUP COMPLETE");
}

int distance(int pin_index){ // measured in cm, returns a float
  int value = analogRead(pin_index);
  return 11235.5 / pow((float)value, 1.1971);
}
bool is_distance_greater_than(int x, int pin_index){
  return x <= distance(pin_index);
}

void loop() {
  switch (bowling_states)
  {
    case 0:
    search_for_bowling_pin();
    case 1:
    attack_bowling_pin();
    case 2:
    finish_attack_bowling_pin();
    case 3:
    find_line_backwards();
  }
}

// FUNCTIONS FOR THE BOWLING PIN
// TO START, PLAY THE FUNCTION search_for_bowling_pin()

void find_line_backwards(){
  forwardcorrection(-0.5);
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
  forwardInMetersTrapeze(0.15);
  bowling_states = 3;
  
}
void attack_bowling_pin(){
  forwardcorrection(0.5);
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
  // Function to go forward without knowing the distance ;
  if (!is_distance_greater_than(15, pin_detect_proxim_av)){
      // Function to go 90 degrees to the left ;
    }
}

void clear_the_wall(){
  // Function to go forward knowing the current distance ;
  if (is_distance_greater_than(130, pin_detect_proxim_cote)){
      // Move a bit to not go in the wall ;
      // Function to go 90 degrees to the right ;
    }
}

void through_the_wall(){
  // Function to go forward a distance big enough for the back of the robot to pass the wall (so a predetermined distance);
  // Function to go 90 degrees to the right ;
  // Function to go forward until finds black line (suiveur de ligne);
  // Function to go forward half a robot of distance (so the middle of the robot is in the middle of the line);
  // Function to go 90 degrees to the left (or until the line is found again);
    }
