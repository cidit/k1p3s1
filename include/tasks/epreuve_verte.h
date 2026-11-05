#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_verte

TASK_CONTEXT()
{
  bool &green;
};

TASK_INIT(bool &green)
{
  return (epreuve_verte_context){
    .green = green
    };
}

TASK_SETUP()
{
  //setup
  return true;
}

TASK_LOOP()
{
  //loop de la tâche
  return false;
}

TASK_CLEANUP()
{
  green_done = true;
  return false;
}

#undef TASK_TYPE