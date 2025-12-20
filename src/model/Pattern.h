#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "Event.h"

class Pattern
{
public:
    Pattern(juce::uint8 id);
    ~Pattern() = default;

    [[nodiscard]] juce::uint8 get_id() const;

private:
    juce::uint8 id;
    juce::MidiMessage recording;
    Event event;
};