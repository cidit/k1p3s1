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

#define TASK_TYPE detect_color

TASK_CONTEXT()
{
  TimerContext timer;
  int &couleur;
  uint16_t &red, &green, &blue;
};

TASK_INIT(int &couleur, uint16_t &red, uint16_t &green, uint16_t &blue)
{
  return (detect_color_context){
    .timer = make_timer(us_from_s(0.1), 0),
    .couleur = couleur,
    .red = red,
    .green = green,
    .blue = blue};
}

TASK_SETUP()
{
  Serial.println("Color View Test!");
  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ; // halt!
  }
  return true;
}

enum color
{
  UNKNOWN_COLOR,
  ROSE,
  ORANGE,
  JAUNE,
  VERT,
  BLEU
};

TASK_LOOP()
{
  int range = 50;
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false); // turn on LED

  if (timer_ready(ctx.timer, micros()))
  {
    timer_reset(ctx.timer, micros());

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true); // turn off LED

    Serial.print("R:\t");
    Serial.print(red);
    Serial.print("\tG:\t");
    Serial.print(green);
    Serial.print("\tB:\t");
    Serial.println(blue);

    if ((red - 780) < range and (green - 580) < range and (blue - 470) < range){
      ctx.couleur = ROSE;
    }
    else if ((red - 690) < range and (green - 575) < range and (blue - 330) < range){
      ctx.couleur = ORANGE;
    }
    else if ((red - 690) < range and (green - 780) < range and (blue - 390) < range){
      ctx.couleur = JAUNE;
    }
    else if ((red - 535) < range and (green - 780) < range and (blue - 590) < range){
      ctx.couleur = VERT;
    }
    else if ((red - 280) < range and (green - 510) < range and (blue - 480) < range){
      ctx.couleur = BLEU;
    }
    else{
      ctx.couleur = UNKNOWN_COLOR;
    }
    Serial.println(ctx.couleur);
  } // takes 50ms to read

  return false;
}

TASK_CLEANUP()
{
  // nothing to do
  return true;
}

#undef TASK_TYPE