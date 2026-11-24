#include <Arduino.h>
#include <LibRobus.h>
#include <stdlib.h>
#include <time.h>

constexpr float WHEEL_DIAMETER = 76.5; // mm
constexpr float WHEEL_DISTANCE_OUTER = 190.0; // mm
constexpr float WHEEL_THICKNESS = 6.65; // mm
constexpr float WHEEL_TICK_PER_TURN = 3200.0;
constexpr float WHEEL_DISTANCE = WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS;
const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue * 3.1416; // m
const float ticks_per_turn = 3200.0; //ticks

float last_pos = 0.0;

float random_float_generator() {
    float n = (rand() % 201 - 100) / 100.0;
    return n;
}

void rotationPID(float degrees)
{
  float offset = 5.0 * (degrees / abs(degrees)); // 3b = 5.0 3a = 4.3
  float target = ((degrees + offset) / 360.0) * circle * (ticks_per_turn / circumference);
  float kp = 0.002; // 3b = 0.002 3a = 0.003
  float ki = 0.000001;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  while (abs(target - ENCODER_Read(0)) > 30 or abs(target + ENCODER_Read(1)) > 30)
  {

    float error0 = target - ENCODER_Read(0);
    float error1 = target + ENCODER_Read(1);

    float derivative0 = error0 - previous_error0;
    float derivative1 = error1 - previous_error1;

    float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
    float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

    if (speed0 > 0.3)
      speed0 = 0.3;
    else if (speed0 < -0.3)
      speed0 = -0.3;

    if (speed1 > 0.3)
      speed1 = 0.3;
    else if (speed1 < -0.3)
      speed1 = -0.3;

    MOTOR_SetSpeed(0, speed0);
    MOTOR_SetSpeed(1, speed1);
    accumulated_error0 += error0;
    accumulated_error1 += error1;
    previous_error0 = error0;
    previous_error1 = error1;
    delay(10);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

bool nearly_equal(float a, float b, float tolerance = 0.0001) {
    return fabs(a - b) < tolerance;
}

float boundary_set(float last_pos, float increment) {
    return abs(increment) / increment - last_pos;
}
void turret_turns(float shooting_arc) {
    float increment = random_float_generator();  // generates float between -1.0 and 1.0
    float boundary = boundary_set(last_pos, increment); // checks relative distance between last position and the boundary of the aim box in respect to the direction of the increment
    float new_aim = abs(boundary) * increment;
    float new_pos = last_pos + new_aim; // calculates new position as the sum of the last position and and a proportion of the boundary
    float turn = new_aim * shooting_arc / 2;
    last_pos = new_pos;
    rotationPID(turn);
}



void boundary_test() {
    float increment = 0.5;
    float last_pos = 0.0;
    float boundary_target = 1.0;
    float boundary_test = boundary_set(last_pos, increment);
    if (nearly_equal(boundary_test, boundary_target)) {
        Serial.println("Boundary 1 ok");
    } else {
        Serial.println("Error : false boundary 1");
    }

    increment = -0.5;
    last_pos = 0.0;
    boundary_target = -1.00;
    boundary_test = boundary_set(last_pos, increment);
    if (nearly_equal(boundary_test, boundary_target)) {
        Serial.println("Boundary 2 ok");
    } else {
        Serial.println("Error : false boundary 2");
    }

    increment = -0.5;
    last_pos = 0.5;
    boundary_target = -1.50;
    boundary_test = boundary_set(last_pos, increment);
    if (nearly_equal(boundary_test, boundary_target)) {
        Serial.println("Boundary 3 ok");
    } else {
        Serial.println("Error : false boundary 3");
    }

    increment = 0.5;
    last_pos = 0.8;
    boundary_target = 0.20;
    boundary_test = boundary_set(last_pos, increment);
    Serial.println("Test 4 : " + String(boundary_test));
    if (nearly_equal(boundary_test, boundary_target)) {
        Serial.println("Boundary 4 ok");
    } else {
        Serial.println("Error : false boundary 4");
    }

    increment = -0.5;
    last_pos = -0.8;
    boundary_target = -0.20;
    boundary_test = boundary_set(last_pos, increment);
    Serial.println("Test 5 : " + String(boundary_test));
    if (nearly_equal(boundary_test, boundary_target)) {
        Serial.println("Boundary 5 ok");
    } else {
        Serial.println("Error : false boundary 5");
    }
}

void setup() {
    BoardInit();
    srand(time(NULL));
    Serial.println("--");
    boundary_test();
}


void loop() {
    /* Serial.print(String(random_float_generator()));
    
    delay(1000); */
    turret_turns(120.0);
    delay(1000);
}

// check for potential code breakdown when last_pos = 1.0 or -1.0