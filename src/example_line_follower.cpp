#include <Arduino.h>
#include <LibRobus.h>

#include <utils/misc.h>
#include <utils/task.h>
#include <utils/timer.h>

#include <line_follower.h>

#include <tasks/track_speed.h> 

TimerContext graph_timer = make_timer(500, 0);
TimerContext encoder_sampling = make_timer(10, 0);
TimerContext pid_timer = make_timer(10, 0);

RingBuffer<2> right_enco = make_ring_buffer<2>();
RingBuffer<2> left_enco = make_ring_buffer<2>();

EmaContext right_speed_ema = make_EMA(encoder_sampling.period, .01);
EmaContext left_speed_ema = make_EMA(encoder_sampling.period, .01);

PidContext right_speed_pid = make_pid(PidK{.p = 0, .i = 0, .d = 0}, D_ON_PV);
PidContext left_speed_pid = make_pid(PidK{.p = 0, .i = 0, .d = 0}, D_ON_PV);

Motor right_motor = make_motor(RIGHT,
                               false,
                               right_speed_pid,
                               right_speed_ema,
                               right_wheel);

Motor left_motor = make_motor(LEFT,
                              false,
                              left_speed_pid,
                              left_speed_ema,
                              left_wheel);

Drive drive = (Drive){
    .left = left_motor,
    .right = right_motor,
};




uint8_t follower_pins[] = { A2, A3, A4 };
constexpr size_t follower_pins_len = sizeof(follower_pins)/sizeof(follower_pins[0]);

MappingParameters mappings[follower_pins_len] = {
    {.min = 280, .max = 730},
    {.min = 50, .max = 500},
    {.min = 60, .max = 640},
};


// MAKE_TASK(TRACK_SPEED,)

void setup() {
    BoardInit();
    Serial.begin(115200);

    for (auto p : follower_pins) {
        pinMode(p, INPUT);
    }
}

void loop() {
    uint16_t raw[follower_pins_len];
    for (size_t i = 0; i < follower_pins_len; i++) {
        raw[i] = analogRead(follower_pins[i]);
    }

    int16_t offset[follower_pins_len];
    pipe_offset(mappings, raw, offset, follower_pins_len);

    float scaled[follower_pins_len];
    pipe_scaled(mappings, offset, scaled, follower_pins_len);

    float clamped[follower_pins_len];
    pipe_clamped(mappings, scaled, clamped, follower_pins_len);

    Serial.println();

    GRAPH(raw[0])
    GRAPH(raw[1])
    GRAPH(raw[2])
    GRAPH(offset[0])
    GRAPH(offset[1])
    GRAPH(offset[2])
    GRAPH(scaled[0])
    GRAPH(scaled[1])
    GRAPH(scaled[2])
    GRAPH(clamped[0])
    GRAPH(clamped[1])
    GRAPH(clamped[2])


    auto intensity_ = intensity(clamped, follower_pins_len);
    auto distribution_ = distribution(clamped, follower_pins_len);
    
    GRAPH(intensity_)
    GRAPH(distribution_)

    delay(200);
}