//fonctions.cpp
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

  while (abs(target - ENCODER_Read(0)) > 10 and abs(target - ENCODER_Read(1)) > 10) {
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
  float offset = 0.0*(degrees/abs(degrees));
  float target = ((degrees+offset)/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.000001;
  float ki = 0.0000000959;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);
  
  
  while (abs(target - ENCODER_Read(0)) > 5 && abs(target + ENCODER_Read(1)) > 5){

  float error0 = target - ENCODER_Read(0);
  float error1 = target + ENCODER_Read(1);

  float derivative0 = error0 - previous_error0;
  float derivative1 = error1 - previous_error1;
  
  float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
  float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));
  //float speed0 = (error0 * kp) + (accumulated_error0 * ki);
  //float speed1 = -((error1 * kp) + (accumulated_error1 * ki));
  //float speed0 = error0 * kp;
  //float speed1 = -error1 * kp;

  MOTOR_SetSpeed(0, speed0);
  MOTOR_SetSpeed(1, speed1);
  accumulated_error0 += error0;
  accumulated_error1 += error1;
  previous_error0 = error0;
  previous_error1 = error1;
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
void turn_back_right(){
  rotationPID(91);
}
void turn_back_left(){
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
    if(speed < abs(target_speed) and millis() - start >= 50 and abs((ENCODER_Read(0) + ENCODER_Read(1))/2) < abs(target) * 0.3){
      //Accélère
    speed +=0.02 + it * 0.005 * signe;
    start = millis();
    it ++;
    }
    else if(abs((ENCODER_Read(0) + ENCODER_Read(1))/2) > abs(target) * 0.6 and millis() - start >= 50 and abs(speed) > 0.2){
      speed -=0.05 * signe;
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
    if (digitalRead(pin_vert) or digitalRead(pin_rouge)){
        return true;
    }
    else{
        return false;
    }
}

void reset_bool(bool bool_x[]){
  for(int i = 0; i < 3; i++){
    bool_x[i] = 0;
  }
}

float difference_read(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time)
{
    /**
     * @brief Calcule la différence entre les points de deux fonctions d'entrées analog IN
     * @details Fait la moyenne mobile simple de deux séries de points, puis les soustrait
     * @param pin_ambiant La pin analog IN connectée à la sortie du circuit pour le bruit ambiant
     * @param pin_5khz La pin analog IN connectée à la sortie du circuit pour l'amplificateur de 5 kHz 
     * @param interval Le temps d'attente entre deux appels de la fonction
     * @param sample_size La quantité de lectures analog pour faire la moyenne des points
     * @param sample_time Le temps d'attente entre deux séries de lectures analog
     * @return La différence entre les aires sous la courbe des deux fonctions d'entrées analog IN (float)
     */
    float sum_ambiant = 0;
    float sum_5khz = 0;

    for (int i = 0; i < sample_size; i++) 
    {
        int reading_ambiant = analogRead(pin_ambiant);
        int reading_5khz = analogRead(pin_5khz);
        sum_ambiant += reading_ambiant;
        sum_5khz += reading_5khz;
        delay(sample_time);
    }

    float average_reading_ambiant = sum_ambiant / sample_size; 
    float average_reading_5khz = sum_5khz / sample_size;

    float difference = average_reading_5khz - average_reading_ambiant;

    delay(interval);

    return difference;
}

bool is_sifflet(int pin_ambiant, int pin_5khz, int patience, float difference_thr) 
{
    /**
     * @brief Vérifie si un sifflet a été entendu
     * @details Additionne difference_read() à une somme sur un certain nombre d'itérations, en regardant à chaque fois si cette somme atteint un certain difference_thr
     * @param pin_ambiant La pin analog IN connectée à la sortie du circuit pour le bruit ambiant
     * @param pin_5khz La pin analog IN connectée à la sortie du circuit pour l'amplificateur de 5 kHz
     * @param patience Le nombre de mises à jour permises à difference_sum pour atteindre difference_thr
     * @param difference_thr Le minimum requis pour que la fonction retourne True
     * @return Un booléen
     */
    float difference_sum = 0.0;
    for (int i = 0; i < patience; i++) {
        difference_sum += difference_read(pin_ambiant, pin_5khz, 100, 5, 5);
        //Serial.print(String(difference_sum));
        if (difference_sum > difference_thr) {
            return true;
        }
    }
    Serial.print("\n");
    return false;
}

ColoredLED::ColoredLED(int led_pin)
  : pin(led_pin), time_set(0), is_on(false)
  {
  }

void ColoredLED::begin()
{
  pinMode(pin, OUTPUT);
}

void ColoredLED::on()
{
  digitalWrite(pin, HIGH);
  time_set = millis();
  is_on = true;
}

void ColoredLED::off()
{
  digitalWrite(pin, LOW);
  is_on = false;
}

bool ColoredLED::is_time()
{
  return is_on and (millis() - time_set > 10 * 1000);
}

ColoredLED colored_leds[nb_leds] = {
    ColoredLED(2),
    ColoredLED(3),
    ColoredLED(4),
    ColoredLED(5),
    ColoredLED(6)
};

void start_colored_leds(ColoredLED colored_leds[])
{
        for (int i = 0; i < nb_leds; i++)
    {
        colored_leds[i].begin();
        colored_leds[i].on();
        delay(1000);
    }
}

void check_colored_leds(ColoredLED colored_leds[])
{
    for (int i = 0; i < nb_leds; i++)
{
    if (colored_leds[i].is_time())
    {
        colored_leds[i].off();
    }
}
}