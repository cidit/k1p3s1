#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
// #include "fonctions.h"
#include <Wire.h>
#include "utils/recepteurIR.h"

#define TASK_TYPE epreuve_bleue

TASK_CONTEXT()
{
  // Mettre les variables qui seront réutilisées dans le main
  bool &robot_reveil;
  bool &blue_done;
};

TASK_INIT(bool &blue_done, bool &robot_reveil)
{
  return (TASK_ctx) {
      // Mettre les variables qui seront déclarées plus bas
      // format: .nom_variable = nom variable,
      .robot_reveil = robot_reveil,
      .blue_done = blue_done,
  };
}

TASK_SETUP()
{
  // setup
  return true;
}

TASK_LOOP()
{
  while (!ctx.robot_reveil)
  {
    if (IR_wake_check())
    {
      beep(3);
      ctx.robot_reveil = true;
    }
  }
  return true;
}

TASK_CLEANUP()
{
  ctx.blue_done = true;
  Serial.println("Épreuve bleue finie");
  return true;
}

#undef TASK_TYPE