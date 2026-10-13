#include <Arduino.h>
#include <LibRobus.h>
#include <Decodeur.h>
#include "utils/timer.h"
#include "utils/ema.h"

Decodeur cmd(&Serial);

TimerContext ema_timer = make_timer(10, 0);
TimerContext graph_timer = make_timer(500, 0);

EmaContext ambiant_ema = make_EMA(ema_timer.period_ms, .1);
EmaContext f5kHz_ema = make_EMA(ema_timer.period_ms, .1);


void setup() 
{
  BoardInit();
  Serial.begin(115200);
}

void loop() 
{
  auto now = millis();
  cmd.refresh();

  if (timer_ready(ema_timer, now)) {
    auto periods_elapsed = timer_update_fixed(ema_timer, now);
    
    auto noise_ambiant = analogRead(A0);
    auto noise_5kHz = analogRead(A3);

    // we're catching up on the missed periods. since we dont know the data we missed,
    // we add n-periods amount of times the new data.
    for (auto i = 0; i < periods_elapsed; i++) {
      EMA_add(ambiant_ema, noise_ambiant);
      EMA_add(f5kHz_ema, noise_5kHz);
    }
  }


  if (timer_ready(graph_timer, now)) {
    timer_reset(graph_timer, now);

    auto ambiant = ambiant_ema.current;
    auto f5kHz = f5kHz_ema.current;

    GRAPH(ambiant)
    GRAPH(f5kHz)
  }
}


/*

// NOTES:
1. keep track of whether or not the wheels are spinning (torqued). if not, apply an initial torque of 
max voltage for a single loop, ***just to kick it out of the static state where the friction 
coeficient is highest*** 

- KICKSTART boolean field on the motor struct. 
  set kickstart at true and voltage at max when applying a speed.
  set it back to false when we detect an encoder change and apply the
    actually requested voltage instead.



*/