#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_jaune

    const int pin_detect_proxim_av = 1;
    const int cote = 2;
    float speed = -0.5;


TASK_CONTEXT()
{
  bool &yellow;
};

TASK_INIT(bool &yellow)
{
  pinMode(pin_detect_proxim_av, INPUT);
  pinMode(cote, INPUT);
  return (epreuve_jaune_context){
    .yellow = yellow
};
}

TASK_SETUP()
{
  //setup
  return true;
}

TASK_LOOP()
{
  return find_the_wall(speed, pin_detect_proxim_av);
}

TASK_CLEANUP()
{
  yellow_done = true;
  return false;
}

#undef TASK_TYPE