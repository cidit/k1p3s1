#pragma once
#include <Arduino.h>

template<size_t len>
struct RingBuffer {
    float buffer[len];
    size_t idx;
    bool full;
};

template<size_t len>
RingBuffer<len> make_ring_buffer() {
    return RingBuffer<len> {
        .buffer = {0},
        .idx = 0,
        .full = false,
    };
}

template<size_t len>
void ring_buffer_append(RingBuffer<len> &rb, float value) {
    rb.buffer[rb.idx] = value;
    rb.idx +=1;
    rb.idx %= len;
    if (!rb.full && rb.idx == 0) {
        rb.full = true;
    }
}

template<size_t len>
float ring_buffer_read_nth(const RingBuffer<len> &rb, size_t n) {
    auto actual_idx = (rb.idx + n) % rb.len;
    return rb.buffer[actual_idx]; 
}