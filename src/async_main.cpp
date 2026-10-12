#include <Arduino.h>
#include <LibRobus.h>
#include "utils/timer.h"
#include "utils/ema.h"

TimerContext ema_timer = make_timer(10, 0);
EmaContext ema_ambiant = make_EMA(ema_timer.period_ms, .1);
EmaContext ema_5kHz = make_EMA(ema_timer.period_ms, .1);


void setup() 
{
  Serial.begin(115200);
  // BoardInit();

  // while (!Serial)
  // {
  //   /* Do nothing */
  // }
}

void loop() 
{
  auto now = millis();
  if (timer_ready(ema_timer, now)) {
    auto dt = int(timer_delta(ema_timer, now));
    auto rollovers = timer_update_fixed(ema_timer, now);
    
    auto noise_ambiant = analogRead(A0);
    auto noise_5kHz = analogRead(A3);

    EMA_add(ema_ambiant, noise_ambiant);
    EMA_add(ema_5kHz, noise_5kHz);


    GRAPH(noise_ambiant)
    GRAPH(ema_ambiant.current)
    GRAPH(noise_5kHz)
    GRAPH(ema_5kHz.current)
    GRAPH(dt)
    GRAPH(rollovers)
  }
}


/*

// NOTES:
1. keep track of whether or not the wheels are spinning (torqued). if not, apply an initial torque of 
max voltage for a single loop, ***just to kick it out of the static state where the friction 
coeficient is highest*** 


*/