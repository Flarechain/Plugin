#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MidiNormalizer
{
public:
    explicit MidiNormalizer(const int max_midi_duration_seconds) : MAX_MIDI_DURATION_SECONDS(max_midi_duration_seconds) {}
    ~MidiNormalizer() = default;

    /// Normalizes a MIDI sequence:
    /// - filters MIDI events, using `is_relevant_midi_message()`
    /// - sorts MIDI events in ascending order
    /// - trims any silence at the start, shifting all events so the first event starts at timestamp 0
    /// - trims the MIDI sequence so that no event exceeds `MAX_MIDI_DURATION_SECONDS`
    void normalize(juce::MidiMessageSequence& midi) const;

    /// Returns true only for the following MIDI messages:
    ///
    /// Note On/Off, Pitch Wheel, Aftertouch, Modulation Wheel,
    /// Sustain Pedal On/Off, Sostenuto Pedal On/Off, Soft Pedal On/Off
    static bool is_relevant_midi_message(const juce::MidiMessage& message);

private:
    const int MAX_MIDI_DURATION_SECONDS;
};