#pragma once

#include <juce_core/juce_core.h>

enum class Instrument
{
    Guitar,
    Keyboard,
    Drums
};

inline juce::String toString(const Instrument instrument)
{
    switch (instrument)
    {
        case Instrument::Guitar:  return "Guitar";
        case Instrument::Keyboard:  return "Keyboard";
        case Instrument::Drums:  return "Drums";
    }
    return "";
}
