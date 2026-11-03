#include "tasks/detect_color.h"
#include "tasks/idle.h"
#include "utils/misc.h"

int couleur;
uint16_t red, green, blue;

MAKE_TASK(detect_color, couleur, red, green, blue)
MAKE_TASK(IDLE)

Task current_task = IDLE_TASK;

void setup()
{
  BoardInit();
  Serial.begin(115200);
  detect_color_TASK.setup(detect_color_TASK.context);
}

void loop()
{
  run_task(detect_color_TASK);
  /*
  if(couleur == ROSE && current_task.state == DONE) 
  {
    EPREUVE_ROSE_TASK.state = SETUP;
    current_task = EPREUVE_ROSE_TASK;
  }

  if (current_task.state != DONE)
  {
    run_task(current_task);
  } 
  else 
  {
    current_task = IDLE_TASK;
  }
  */
}