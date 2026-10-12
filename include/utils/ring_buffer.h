#pragma once
#include <Arduino.h>

struct RingBuffer {
    float *buffer;
    size_t len;
    size_t idx;
    bool full;
};

RingBuffer make_ring_buffer(size_t length) {
    return RingBuffer {
        .buffer = {0},
        .len = length,
        .idx = 0,
        .full = false,
    };
}

void ring_buffer_append(RingBuffer &rb, float value) {
    rb.buffer[rb.idx] = value;
    rb.idx +=1;
    rb.idx %= rb.len;
    if (!rb.full && rb.idx == 0) {
        rb.full = true;
    }
}

float ring_buffer_read_nth(RingBuffer &rb, size_t n) {
    auto actual_idx = (rb.idx + n) % rb.len;
    return rb.buffer[actual_idx]; 
}