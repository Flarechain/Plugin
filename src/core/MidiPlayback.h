#pragma once

#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../model/Pattern.h"

/// An engine for MIDI playback.
class MidiPlayback
{
public:
    MidiPlayback() : state(NotPlaying), midi_index(0), sample_offset(0), sample_rate(44100) {}
    ~MidiPlayback() = default;

    [[nodiscard]] bool is_playing() const { return state != NotPlaying; }

    void set_sample_rate(double sample_rate) { this->sample_rate = sample_rate; }

    [[nodiscard]] std::optional<PatternId> get_playing_pattern() const { return playing_pattern; }

    /// Returns the next MIDI buffer to be passed to processBlock during playback.
    ///
    /// The parameter `buffer_size` must match the number of samples of the current audio buffer
    /// (i.e., AudioBuffer::getNumSamples()).
    /// This is required because some DAWs (like FL Studio) may use dynamically sized buffers instead of
    /// fixed samplesPerBlock.
    [[nodiscard]] juce::MidiBuffer get_next_buffer(int buffer_size);

    /// Plays the given midi.
    void play(PatternId id, juce::MidiMessageSequence midi);

    /// Stops the current playback.
    void stop();

    std::function<void(PatternId id)> on_play;
    std::function<void(PatternId id)> on_stop;

private:
    enum State
    {
        Playing,
        Stopping,   // still playing, but sends the last buffer releasing notes and controls
        NotPlaying
    };

    std::optional<PatternId> playing_pattern;
    juce::MidiMessageSequence playing_midi;

    std::optional<PatternId> pending_pattern;
    juce::MidiMessageSequence pending_midi;

    std::atomic<State> state;
    bool active_notes[16][128] = {};

    std::atomic<int> midi_index;
    std::atomic<int> sample_offset;

    std::atomic<double> sample_rate;

    /// Resets the current playing data.
    ///
    /// Clears the MIDI sequence, sets the `playing_pattern` to `std::nullopt` and resets playback indices.
    void reset_playing();

    /// Promotes the pending pattern to playing and clears the pending pattern's data.
    void pending_to_playing();
};
