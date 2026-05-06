#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "../core/MidiPlayback.h"
#include "../core/MidiRecording.h"
#include "../core/RealtimeBuffer.h"
#include "../core/MidiNormalizer.h"
#include "../model/PatternList.h"

class FlarechainAudioProcessor final : public juce::AudioProcessor, juce::AsyncUpdater
{
public:
    FlarechainAudioProcessor();
    ~FlarechainAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void handleAsyncUpdate() override;

    const PatternList& get_pattern_list() const { return pattern_list; }
    const Pattern& get_pattern(const PatternId id) const { return pattern_list.get(id); }

    /// Returns the IDs of patterns that contain MIDI data,
    /// i.e., patterns that were populated by recording MIDI sequence or by importing a MIDI file.
    std::set<PatternId> get_active_patterns() const;

    /// Returns the IDs of active patterns with missing or incomplete IP address.
    std::set<PatternId> get_patterns_with_invalid_ip() const;

    /// Returns the IDs of active patterns with missing or invalid OSC message.
    std::set<PatternId> get_patterns_with_invalid_osc() const;

    /// Returns whether the AI model has been downloaded.
    bool is_model_downloaded() const { return false; }

    /// Sets the MIDI sequence for the specified pattern. The given MIDI is normalized before being stored in the pattern.
    void set_midi(PatternId id, juce::MidiMessageSequence midi);

    /// Sets the given IP address to the specified pattern.
    void set_ip_address(PatternId id, std::optional<juce::IPAddress> ip) const;

    /// Sets the given OSC message to the specified pattern.
    void set_osc_message(PatternId id, std::optional<juce::OSCMessage> osc) const;

    /// Deletes MIDI and event's data for the specified pattern.
    void delete_pattern(PatternId id);

    /// Plays the specified pattern, sending its MIDI data in output.
    void play_pattern(PatternId id);

    /// Stops the playback of the current pattern.
    void stop_playing();

    /// Records the specified pattern.
    void record_pattern(PatternId id);

    /// Stops the recording of the current pattern, updating its MIDI sequence with the recorded one.
    void stop_recording();

    /// Tries to convert the content of a file into a MIDI sequence.
    ///
    /// Will return `std::nullopt` if the file has no valid MIDI data.
    ///
    /// If the MIDI file has data on multiple tracks, all messages will be merged into a single MIDIMessageSequence.
    ///
    /// The timestamps will be converted into seconds instead of ticks.
    static std::optional<juce::MidiMessageSequence> load_midi(const juce::File& file);

    std::function<void(PatternId id)> on_playback_start;
    std::function<void(PatternId id)> on_playback_stop;
    std::function<void(PatternId id)> on_recording_start;
    std::function<void(PatternId id)> on_recording_stop;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlarechainAudioProcessor)

    struct AsyncEvent
    {
        enum class Type : int
        {
            None,
            PlaybackStart,
            PlaybackStop,
            RecordingStart,
            RecordingStop
        };

        Type type;
        PatternId pattern_id;
    };

    PatternList pattern_list;
    MidiPlayback playback;
    MidiRecording recording;
    MidiNormalizer midi_normalizer;

    RealtimeBuffer<AsyncEvent, 16> async_event_queue;

    // maximum midi duration for every pattern
    static constexpr juce::uint8 MAX_MIDI_DURATION_SECONDS = 10;
};
