#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_verte

TASK_CONTEXT()
{
  bool &green_done;
};

TASK_INIT(bool &green_done)
{
  return (TASK_ctx){
    .green_done = green_done
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
  ctx.green_done = true;
  return false;
}

#undef TASK_TYPE