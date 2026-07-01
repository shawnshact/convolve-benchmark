//
// Created by shawnshact on 6/29/26.
//

#ifndef CONVOLVETEST_RINGBUFFER_H
#define CONVOLVETEST_RINGBUFFER_H
#include <optional>
#include <vector>

namespace buffer {
    constexpr size_t DEFAULT_MAX_BUFFER_SIZE = 32;
    template <typename T> class RingBuffer {
        std::vector<T> buffer;
        size_t capacity;
        int head;
        int tail;
        size_t currentSize;

        public:
            RingBuffer();

            explicit RingBuffer(size_t maxSize);
            ~RingBuffer();

            // Public function for getting max size of ring buffer
            [[nodiscard]] size_t getMaxBufferSize() const;

            // Pushes an element to buffer. Replaces oldest element if buffer full
            void push(T data);

            // Pops an element from buffer
            std::optional<T> pop();

            // Gets front of buffer without popping
            T peek();

            // Checks if buffer is empty
            [[nodiscard]] bool isEmpty() const;

            [[nodiscard]] bool isFull() const;


    };


} // ringbuffer

#endif //CONVOLVETEST_RINGBUFFER_H
