#pragma once

#include "utils/task.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>

#define TASK_TYPE epreuve_orange

TASK_CONTEXT()
{
  //Mettre les variables qui seront réutilisées dans le main
};

TASK_INIT()
{
  return (epreuve_orange_context){
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
  // nothing to do
  return false;
}

#undef TASK_TYPE