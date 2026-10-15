#include "fonctions.h"
#include <LibRobus.h>


void forwardInMeters(float meters) {
  float offset = 0.0;
  float target = ((meters+offset) / circumference) * ticks_per_turn;

  float kp = 0.00006;
  float ki = 0.00000001;
  float kp_correction = 0.005; 

  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  while (abs(target - ENCODER_Read(0)) > 10 or abs(target - ENCODER_Read(1)) > 10) {
    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);

    float error0 = target - count_0;
    float error1 = target - count_1;

    float speed0 = (error0 * kp) + (accumulated_error0 * ki);
    float speed1 = (error1 * kp) + (accumulated_error1 * ki);

    float diff = (float)(count_0 - count_1);
    float correction = diff * kp_correction;

    MOTOR_SetSpeed(0, speed0 - correction);
    MOTOR_SetSpeed(1, speed1 + correction);

    accumulated_error0 += error0;
    accumulated_error1 += error1;

  }

  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0); 
}
void rotationPID(float degrees){
  float offset = 5.0*(degrees/abs(degrees)); // 3b = 5.0 3a = 4.3
  float target = ((degrees+offset)/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.002; // 3b = 0.002 3a = 0.003
  float ki = 0.000001;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);
  
  
  while (abs(target - ENCODER_Read(0)) > 30 or abs(target + ENCODER_Read(1)) > 30){

  float error0 = target - ENCODER_Read(0);
  float error1 = target + ENCODER_Read(1);

  float derivative0 = error0 - previous_error0;
  float derivative1 = error1 - previous_error1;
  
  float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
  float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

  if (speed0 > 0.3) speed0 = 0.3;
  else if (speed0 < -0.3) speed0 = -0.3;

  if (speed1 > 0.3) speed1 = 0.3;
  else if (speed1 < -0.3) speed1 = -0.3;

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
void finalDance(float degrees) {
  float target = (degrees / 360.0) * circle * (ticks_per_turn / circumference);
  
  float kp = 0.05;
  float kd = 0.01;
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while (abs(target - ENCODER_Read(0)) > 10 || abs(target + ENCODER_Read(1)) > 10) {

    float error0 = target - ENCODER_Read(0);
    float error1 = target + ENCODER_Read(1);

    float derivative0 = error0 - previous_error0;
    float derivative1 = error1 - previous_error1;

    float speed0 = (error0 * kp) + (derivative0 * kd);
    float speed1 = -((error1 * kp) + (derivative1 * kd));

    speed0 = constrain(speed0, -1.0, 1.0);
    speed1 = constrain(speed1, -1.0, 1.0);

    MOTOR_SetSpeed(0, speed0);
    MOTOR_SetSpeed(1, speed1);
  }

  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}
void turn_right(){
  rotationPID(90);
}
void turn_left(){
  rotationPID(-90);
}
void turn_more_right(){
  rotationPID(91);
}
void turn_more_left(){
  rotationPID(-91);
}

void forwardInMetersTrapeze(float meters) {
  float it = 0;
  float target = (meters / circumference) * ticks_per_turn;
  float signe = target / abs(target);
  float kp_correction = 0.005; 
  float target_speed = 0.6;
  float speed = 0.2 * signe; // Initial speed based on direction


  ENCODER_Reset(0);
  ENCODER_Reset(1);
  unsigned long start = millis();

  while (abs(target)>abs((ENCODER_Read(0) + ENCODER_Read(1))/2)) {
    if(abs(speed) < abs(target_speed) and millis() - start >= 50 and abs((ENCODER_Read(0) + ENCODER_Read(1))/2) < abs(target) * 0.3){
      //Accélère
    speed +=0.02 + it * 0.005 * signe;
    start = millis();
    it ++;
    }
    else if(abs((ENCODER_Read(0) + ENCODER_Read(1))/2) > abs(target) * 0.6 and millis() - start >= 50 and abs(speed) > 0.2){
      speed -=(0.022/abs(meters)) * signe;
      start = millis();
    } 
    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);

    float diff = (float)(count_0 - count_1);
    float correction = diff * kp_correction;

    MOTOR_SetSpeed(0, speed- correction);
    MOTOR_SetSpeed(1, speed+ correction);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0); 
}


bool check_open(){
    if (digitalRead(pin_vert) and digitalRead(pin_rouge)){
        return true;
    }
    else{
        return false;
    }
}

void reset_bool(bool bool_x[]){
  for(int i = 0; i < 3; i++){
    if (i == 1) bool_x[i] = 1;
    else bool_x[i] = 0;
  }
}

float average_read(int pin_analog, int sample_size, int sample_time) 
{
    float mean = 0;

    for (int i = 0; i < sample_size; i++)
    {
        int reading = analogRead(pin_analog);
        mean += reading / sample_size;
        delay(int(sample_time / 2));
    }
    return mean;
}

float difference(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time)
{
    float average_reading_ambiant = average_read(pin_ambiant, sample_size, sample_time);
    float average_reading_5khz = average_read(pin_5khz, sample_size, sample_time);

    float difference = average_reading_5khz - average_reading_ambiant;

    delay(interval);

    return difference;
    
/*     Serial.println
    (
    String(">") +
    "avgA:" + String(average_reading_ambiant) + "," +
    "avg5:" + String(average_reading_5khz) + "," +
    "iA:" + String(integral_ambiant) + "," +
    "i5:" + String(integral_5khz) + "," + 
    "id:" + String(integral_difference)
    );  */

}


bool is_sifflet(/* EmaContext f_ambiant, EmaContext f_5khz, */int pin_ambiant, int pin_5khz, int patience, float thr, int interval) 
{
    float sum = 0.0;
    for (int i = 0; i < patience; i++) {
        sum += difference(pin_ambiant, pin_5khz, interval, 5, 10);
        //sum += difference_ema(f_ambiant, f_5khz);
        if (sum > thr / patience * i) {
            return true;
        }
    }
    //Serial.println("false");
    return false;
}