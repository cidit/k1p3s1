#pragma once
#include <Arduino.h>

template<size_t size>
struct RingBuffer {
    float buffer[size];
    size_t idx;
    bool full;
};

template<size_t size>
RingBuffer<size> make_ring_buffer() {
    return RingBuffer<size> {
        .buffer = {0},
        .idx = 0,
        .full = false,
    };
}

template<size_t size>
void ring_buffer_append(RingBuffer<size> &rb, float value) {
    rb.buffer[rb.idx] = value;
    rb.idx +=1;
    rb.idx %= size;
    if (!rb.full && rb.idx == 0) {
        rb.full = true;
    }
}

template<size_t size>
float ring_buffer_read_nth(const RingBuffer<size> &rb, size_t n) {
    auto actual_idx = (rb.idx + n) % size;
    return rb.buffer[actual_idx]; 
}