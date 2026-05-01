#pragma once

template <typename T, size_t Size>
bool RealtimeBuffer<T, Size>::push(const T& item)
{
    int start_1, size_1, start_2, size_2;
    fifo.prepareToWrite(1, start_1, size_1, start_2, size_2);

    if (size_1 == 0) { return false; }

    buffer[start_1] = item;
    fifo.finishedWrite(1);
    return true;
}

template <typename T, size_t Size>
bool RealtimeBuffer<T, Size>::pop(T& item)
{
    int start_1, size_1, start_2, size_2;

    fifo.prepareToRead(1, start_1, size_1, start_2, size_2);

    if (size_1 == 0) { return false; }

    item = buffer[start_1];
    fifo.finishedRead(1);
    return true;
}
