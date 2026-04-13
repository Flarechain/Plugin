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
    [[nodiscard]] juce::MidiMessageSequence get_midi() const { return midi; }
    [[nodiscard]] Event& get_event() { return event; }
    [[nodiscard]] bool has_empty_midi() const { return midi.getNumEvents() == 0; }

    void set_midi(juce::MidiMessageSequence midi) { this->midi = midi; }

    /// Resets the Pattern by clearing its MIDI and event.
    void clear();

private:
    PatternId id;
    PatternName name;
    juce::MidiMessageSequence midi;
    Event event;
};