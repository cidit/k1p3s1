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

TASK_CONTEXT(detect_color)
{
  TimerContext timer;
  int &couleur;
  uint16_t &red, &green, &blue;
};

TASK_INIT(detect_color)(int &couleur, uint16_t &red, uint16_t &green, uint16_t &blue)
{
  return (detect_color_context){
    .timer = make_timer(us_from_s(1), 0),
    .couleur = couleur,
    .red = red,
    .green = green,
    .blue = blue};
}

TASK_SETUP(detect_color)
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

TASK_LOOP(detect_color)
{
  int range = 60;
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

    if ((red - 410) < range and (green - 305) < range and (blue - 245) < range){
      ctx.couleur = ROSE;
    }
    else if ((red - 345) < range and (green - 295) < range and (blue - 175) < range){
      ctx.couleur = ORANGE;
    }
    else if ((red - 365) < range and (green - 400) < range and (blue - 205) < range){
      ctx.couleur = JAUNE;
    }
    else if ((red - 265) < range and (green - 375) < range and (blue - 285) < range){
      ctx.couleur = VERT;
    }
    else if ((red - 155) < range and (green - 275) < range and (blue - 255) < range){
      ctx.couleur = BLEU;
    }
    else{
      ctx.couleur = UNKNOWN_COLOR;
    }
    Serial.println(ctx.couleur);
  } // takes 50ms to read

  return false;
}

TASK_CLEANUP(detect_color)
{
  // nothing to do
  return false;
}