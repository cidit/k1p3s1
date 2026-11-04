#include "tasks/idle.h"
#include "utils/misc.h"
#include "tasks/detect_color.h"
#include "tasks/epreuve_rose.h"
#include "tasks/epreuve_orange.h"
#include "tasks/epreuve_jaune.h"
#include "tasks/epreuve_verte.h"
#include "tasks/epreuve_bleue.h"

int couleur;
uint16_t red, green, blue;

MAKE_TASK(IDLE)
MAKE_TASK(detect_color, couleur, red, green, blue)
MAKE_TASK(epreuve_rose)
MAKE_TASK(epreuve_orange)
MAKE_TASK(epreuve_jaune)
MAKE_TASK(epreuve_verte)
MAKE_TASK(epreuve_bleue)

Task current_task = IDLE_TASK;

void setup()
{
  BoardInit();
  Serial.begin(115200);
  detect_color_TASK.setup(detect_color_TASK.context);
  epreuve_rose_TASK.setup(epreuve_rose_TASK.context);
  epreuve_orange_TASK.setup(epreuve_orange_TASK.context);
  epreuve_jaune_TASK.setup(epreuve_jaune_TASK.context);
  epreuve_verte_TASK.setup(epreuve_verte_TASK.context);
  epreuve_bleue_TASK.setup(epreuve_bleue_TASK.context);
}

void loop()
{
  run_task(detect_color_TASK);
  if (current_task.state == DONE)
  {
    if(couleur == ROSE) 
    {
      epreuve_rose_TASK.state = SETUP;
      current_task = epreuve_rose_TASK;
    }
    else if(couleur == ORANGE) 
    {
      epreuve_orange_TASK.state = SETUP;
      current_task = epreuve_orange_TASK;
    }
    else if(couleur == JAUNE) 
    {
      epreuve_jaune_TASK.state = SETUP;
      current_task = epreuve_jaune_TASK;
    }
    else if(couleur == VERT) 
    {
      epreuve_verte_TASK.state = SETUP;
      current_task = epreuve_verte_TASK;
    }
    else if(couleur == BLEU) 
    {
      epreuve_bleue_TASK.state = SETUP;
      current_task = epreuve_bleue_TASK;
    }
  }

  if (current_task.state != DONE)
  {
    run_task(current_task);
  } 
  else 
  {
    current_task = IDLE_TASK;
  }
}