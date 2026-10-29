#pragma once

#include "utils/task.h"
#include "utils/timer.h"
#include "utils/misc.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

TASK_CONTEXT(detect_couleur) {
  TimerContext timer;
  int &couleur;
};

TASK_INIT(detect_couleur) (int &couleur) {
    return (detect_couleur_context) {
      .timer = make_timer(us_from_s(1), 0),
      .couleur = couleur
    };
}

TASK_SETUP(detect_couleur) {
    Serial.println("Color View Test!");
    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
    return true;
}

TASK_LOOP(detect_couleur) {
  int range = 20;
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED

  if(timer_ready(ctx.timer, micros())){
    timer_reset(ctx.timer, micros());

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED

    Serial.print("C:\t"); Serial.print(clear);
    Serial.print("\tR:\t"); Serial.print(red);
    Serial.print("\tG:\t"); Serial.print(green);
    Serial.print("\tB:\t"); Serial.println(blue);

    if (abs(red - 222) < range and abs(green - 305) < range and abs(blue - 242) < range){
      ctx.couleur = 0; //vert
    } else if(abs(red - 160) < range and abs(green - 260) < range and abs(blue - 245) < range){
      Serial.println("BLEU");
    } else if(abs(red - 310) < range and abs(green - 350) < range and abs(blue - 200) < range){
      Serial.println("JAUNE");
    } else if(abs(red - 310) < range and abs(green - 275) < range and abs(blue - 180) < range){
      Serial.println("ORANGE");
    } else if(abs(red - 376) < range and abs(green - 300) < range and abs(blue - 250) < range){
      Serial.println("ROUGE");
    } else{
      Serial.println("Unknown color");
    }
  } // takes 50ms to read

  return false;
}


TASK_CLEANUP(detect_couleur) {
    // nothing to do
    return false;
}

int couleur;

MAKE_TASK(detect_couleur, couleur)

void setup()
{
  BoardInit();
  Serial.begin(115200);
  detect_couleur_TASK.setup(detect_couleur_TASK.context);
}

void loop()
{
  detect_couleur_TASK.loop(detect_couleur_TASK.context);
}