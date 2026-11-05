#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_bleue

TASK_CONTEXT()
{
    bool &blue;
};

TASK_INIT(bool &blue)
{
  return (epreuve_bleue_context){
     .blue = blue
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
  blue_done = true;
  return false;
}

#undef TASK_TYPE