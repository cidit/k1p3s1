#include <Arduino.h>
#include <LibRobus.h>

#include "utils/timer.h"
#include "utils/ring_buffer.h"
#include "drive.h"
#include "hardware.h"
#include "odometry2wheels.h"

TimerContext graph_timer = make_timer(500, 0);
TimerContext encoder_sampling = make_timer(10, 0);

RingBuffer<2> right_enco = make_ring_buffer<2>();
RingBuffer<2> left_enco = make_ring_buffer<2>();

OdometryContext odometry = make_odometry();


void setup()
{
  BoardInit();
  Serial.begin(115200);
}

void loop()
{
  auto now = millis();

  if (timer_ready(encoder_sampling, now))
  {
    auto periods = timer_update_fixed(encoder_sampling, now);
    for (size_t i = 0; i < periods; i++)
    {
      auto right = ENCODER_Read(RIGHT);
      ring_buffer_append(right_enco, right);
      auto left = ENCODER_Read(LEFT);
      ring_buffer_append(left_enco, left);
    }
  }

  if (timer_ready(graph_timer, now))
  {
    timer_reset(graph_timer, now);

    if (right_enco.full && left_enco.full)
    {
      auto r_delta_mm = ticks_to_mm(
          WHEEL_TICK_PER_TURN,
          WHEEL_DIAMETER,
          ring_buffer_read_nth(right_enco, 1) - ring_buffer_read_nth(right_enco, 0));
      auto r_mps = r_delta_mm / encoder_sampling.period_ms; // mm/ms = m/s

      GRAPH(r_mps)
      auto l_delta_mm = ticks_to_mm(
          WHEEL_TICK_PER_TURN,
          WHEEL_DIAMETER,
          ring_buffer_read_nth(left_enco, 1) - ring_buffer_read_nth(left_enco, 0));
      auto l_mps = l_delta_mm / encoder_sampling.period_ms; // mm/ms = m/s
      GRAPH(l_mps)
      GRAPH(r_mps/l_mps)

      odometry_update(odometry, 
        r_delta_mm, l_delta_mm, 
        WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS
      );

      PLOT("odometry", odometry.position_mm.x, odometry.position_mm.y)
    }
  }
}