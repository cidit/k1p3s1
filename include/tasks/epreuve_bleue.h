#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_bleue

TASK_CONTEXT()
{
  //Mettre les variables qui seront réutilisées dans le main
};

TASK_INIT()
{
  return (epreuve_bleue_context){
    //Mettre les variables qui seront déclarées plus bas
    //format: .nom_variable = nom variable,
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