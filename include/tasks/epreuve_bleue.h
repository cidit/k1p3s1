#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "utils/recepteurIR.h"
#include <Wire.h>

#define TASK_TYPE epreuve_bleue

TASK_CONTEXT()
{
  //Mettre les variables qui seront réutilisées dans le main
  bool &robot_reveil = false;
};

TASK_INIT(bool &robot_reveil)
{
  return (epreuve_bleue_context){
    //Mettre les variables qui seront déclarées plus bas
    //format: .nom_variable = nom variable,
    .robot_reveil = robot_reveil;
    };
}

TASK_SETUP()
{
  //setup
  return true;
}

TASK_LOOP()
{
   while (!robot_reveil)
  {
    if (IR_wake_check())
    {
      beep(3);
      robot_reveil = true;
    }
  }
  return true;
}

TASK_CLEANUP()
{
  Serial.println("Épreuve bleue finie")
  return true;
}

#undef TASK_TYPE