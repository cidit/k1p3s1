#include <Arduino.h>
#include <LibRobus.h>

using timestamp = uint32_t;

struct SystemState {
  timestamp last_run;
};

void setup() 
{
  BoardInit();
}

void loop() 
{
  
}


/*

// NOTES:
1. keep track of whether or not the wheels are spinning (torqued). if not, apply an initial torque of 
max voltage for a single loop, ***just to kick it out of the static state where the friction 
coeficient is highest*** 


*/