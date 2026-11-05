#include <Arduino.h>
#include <LibRobus.h>
#include <fonctions.h>
#include <utils/misc.h>

const int pin_detect_proxim_av = 1;
const int cote = 2;

int attack_angle = 0;
int bowling_states = 0;
int wall_states = 0;
int main_state = 2; // idle = 0, bowling = 1, wall = 2
float speed = -0.5;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("START");
    BoardInit();
    pinMode(pin_detect_proxim_av, INPUT);
    pinMode(cote, INPUT);

}

// FUNCTIONS FOR THE WALL
// TO START, PLAY THE FUNCTION find_the_wall()

void find_the_wall(){
  int sensor = ROBUS_ReadIR(pin_detect_proxim_av);
  Serial.println(sensor);
  forwardcorrection(speed);
  if (ROBUS_ReadIR(pin_detect_proxim_av) >= 350){
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      delay(10);
      rotationPID(-90, 0.2); // Function to go 90 degrees to the left ;
      forwardInMetersTrapeze(-0.5);
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      delay(10);
      rotationPID(90, 0.2); // Function to go 90 degrees to the right ;
      forwardInMetersTrapeze(-0.5); // Temporary, needs to pass the wall, so the distance is determined experimentaly (not 0.25)
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      delay(10);
      rotationPID(90,0.2);
      forwardInMetersTrapeze(-0.5);
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      delay(10);
      rotationPID(-90, 0.2);
    }
}

void loop() {
  find_the_wall();
}