#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#include "utils/timer.h"
#include "utils/misc.h"

#define TASK_TYPE epreuve_rose

TASK_CONTEXT()
{
  //Mettre les variables qui seront réutilisées dans le main
  TimerContext timer;
};

TASK_INIT()
{
  return (epreuve_rose_context){
    //Mettre les variables qui seront déclarées plus bas
    //format: .nom_variable = nom variable,
    .timer = make_timer(us_from_s(5), 0)
    };
}

TASK_SETUP()
{
  //setup
  timer_reset(ctx.timer, micros());
  Serial.print("Épreuve rose en cours");
  return true;
}

TASK_LOOP()
{
  //loop de la tâche
    if (timer_ready(ctx.timer, micros()))
  {
    timer_reset(ctx.timer, micros());
    return true;
  }
  return false;
}

TASK_CLEANUP()
{
  // nothing to do
  Serial.print("Épreuve rose finie");
  return true;
}

#undef TASK_TYPE