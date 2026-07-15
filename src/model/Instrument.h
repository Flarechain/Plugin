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

/// Serializes the event to a ValueTree.
inline juce::ValueTree to_value_tree(const Instrument& instrument)
{
    juce::ValueTree instrument_tree("Instrument");
    juce::String value;
    switch (instrument)
    {
        case Instrument::Guitar:  value = "guitar"; break;
        case Instrument::Keyboard:  value = "keyboard"; break;
        case Instrument::Drums:  value = "drums"; break;
    }
    instrument_tree.setProperty("value", value, nullptr);
    return instrument_tree;
}

/// Restores the event from a ValueTree.
inline std::optional<Instrument> from_value_tree(const juce::ValueTree& state)
{
    if (!state.isValid()) return std::nullopt;

    const juce::String value = state.getProperty("value", "");
    if (!value.isEmpty())
    {
        if (value == "guitar") { return Instrument::Guitar; }
        else if (value == "keyboard") { return Instrument::Keyboard; }
        else if (value == "drums") { return Instrument::Drums; }
    }
    return std::nullopt;
}