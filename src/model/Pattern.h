#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "Event.h"

typedef juce::uint8 PatternId;
typedef juce::String PatternName;

class Pattern
{
public:
    explicit Pattern(PatternId id);
    ~Pattern() = default;

    [[nodiscard]] PatternId get_id() const { return id; }
    [[nodiscard]] PatternName get_name() const { return name; }
    [[nodiscard]] juce::MidiMessage get_midi() const { return midi; }
    [[nodiscard]] bool has_empty_midi() const { return midi.getRawDataSize() > 0; }

private:
    PatternId id;
    PatternName name;
    juce::MidiMessage midi;
    Event event;
};