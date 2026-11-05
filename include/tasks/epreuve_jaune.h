#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_jaune

    const int pin_detect_proxim_av = 1;
    const int cote = 2;

TASK_CONTEXT()
{
  //Mettre les variables qui seront réutilisées dans le main
};

TASK_INIT()
{
  pinMode(pin_detect_proxim_av, INPUT);
  pinMode(cote, INPUT);
  return (epreuve_jaune_context){
};
}

TASK_SETUP()
{
  //setup
  return true;
}

TASK_LOOP()
{
  find_the_wall();
  return false;
}

TASK_CLEANUP()
{
  // nothing to do
  return false;
}

#undef TASK_TYPE