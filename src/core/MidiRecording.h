#pragma once

#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../model/Pattern.h"
#include "MidiNormalizer.h"

/// An engine for recording MIDI sequences.
class MidiRecording
{
public:
    explicit MidiRecording(const int max_midi_duration_seconds) : state(NotRecording), sample_offset(0),
        sample_rate(44100), MAX_MIDI_DURATION_SECONDS(max_midi_duration_seconds) {}

    ~MidiRecording() = default;

    [[nodiscard]] bool is_recording() const { return state == Recording; }

    void set_sample_rate(double sample_rate) { this->sample_rate = sample_rate; }

    [[nodiscard]] std::optional<PatternId> get_recording_pattern() const { return recording_pattern; }

    /// Reads the given MIDI buffer and adds MIDI events to the recorded sequence.
    ///
    /// MIDI events are filtered during recording, using `MidiNormalizer::is_relevant_midi_message()`.
    ///
    /// If the timestamp of a MIDI event exceeds `MAX_MIDI_DURATION_SECONDS`, the recording will stop.
    ///
    /// The parameter `buffer_size` must match the number of samples of the current audio buffer
    /// (i.e., AudioBuffer::getNumSamples()).
    /// This is required because some DAWs (like FL Studio) may use dynamically sized buffers instead of
    /// fixed samplesPerBlock.
    void add_buffer(const juce::MidiBuffer& buffer, int buffer_size);

    /// Records the given pattern.
    void record(PatternId id);

    /// Stops recording the current pattern.
    void stop();

    std::function<void(PatternId id)> on_record;
    std::function<void(PatternId id, juce::MidiMessageSequence)> on_stop;

private:
    enum State
    {
        Recording,
        NotRecording
    };

    std::optional<PatternId> recording_pattern;
    juce::MidiMessageSequence recording_midi;

    std::atomic<State> state;
    bool active_notes[16][128] = {};
    std::atomic<bool> first_note = true;

    std::atomic<juce::uint32> sample_offset;

    std::atomic<double> sample_rate;

    const int MAX_MIDI_DURATION_SECONDS;

    /// Resets the recording data.
    ///
    /// Clears the MIDI sequence, sets the `recording_pattern` to `std::nullopt` and resets recording indices.
    void reset();
};