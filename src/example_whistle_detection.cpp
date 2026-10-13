#include <Arduino.h>
#include "utils/timer.h"
#include "utils/ema.h"

TimerContext ema_timer = make_timer(10, 0);
TimerContext graph_timer = make_timer(500, 0);

EmaContext ambiant_ema = make_EMA(ema_timer.period_ms, .1);
EmaContext f5kHz_ema = make_EMA(ema_timer.period_ms, .1);


void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  auto now = millis();

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
