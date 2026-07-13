#pragma once

#include <juce_core/juce_core.h>

/// Lock-free circular buffer designed for real-time audio threads.
template <typename T, size_t Size>
class RealtimeBuffer
{
public:
    RealtimeBuffer() = default;
    ~RealtimeBuffer() = default;

    bool push(const T& item);
    bool pop(T& item);
    int count() const { return fifo.getNumReady(); }
    void clear() { fifo.reset(); }

private:
    juce::AbstractFifo fifo { Size };
    std::array<T, Size> buffer;
};

#include "RealtimeBuffer.tpp"