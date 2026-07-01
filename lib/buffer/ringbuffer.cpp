//
// Created by shawnshact on 6/29/26.
//

#include "ringbuffer.hpp"

#include <optional>

namespace buffer {
    template<typename T> RingBuffer<T>::RingBuffer(const size_t maxSize) {
        capacity = maxSize;
        head = tail = currentSize = 0;
        buffer(capacity);
    }

    template<typename T> RingBuffer<T>::RingBuffer() {
        RingBuffer(DEFAULT_MAX_BUFFER_SIZE);
    }

    template<typename T> size_t RingBuffer<T>::getMaxBufferSize() const {
        return capacity;
    }

    template<typename T> bool RingBuffer<T>::isEmpty() const {
        return currentSize == 0;
    }

    template<typename T> bool RingBuffer<T>::isFull() const {
        return currentSize == capacity;
    }

    template<typename T> void RingBuffer<T>::push(T data) {
        /*
         * What do here?
         * Scenario 1: Buffer not full, modulo increment tail index, set arr value to tail index, increase currSize by 1
         * Scenario 2: Buffer is full, modulo increment head index, modulo increment head index, set arr value at tail index
         */
        if (isFull()) {
            head = (head + 1) % capacity;
        }
        else {
            currentSize++;
        }
        tail = (tail + 1) % capacity;
        buffer[tail] = data;
    }

    template<typename T> std::optional<T> RingBuffer<T>::pop() {
        /*
         * What do here?
         * If empty, return nothing? Maybe make optional?
         * Otherwise, return head and modulo increment, decrease currSize by 1
         */
        if (isEmpty()) return std::nullopt;
        T bufferHead = buffer[head];
        head = (head + 1) % capacity;
        currentSize--;
        return bufferHead;
    }
} // ringbuffer