#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../model/PatternList.h"

class FlarechainAudioProcessor final : public juce::AudioProcessor
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
    void set_midi(PatternId id, juce::MidiMessageSequence midi) const;

    /// Sets the given IP address to the specified pattern.
    void set_ip_address(PatternId id, std::optional<juce::IPAddress> ip) const;

    /// Sets the given OSC message to the specified pattern.
    void set_osc_message(PatternId id, std::optional<juce::OSCMessage> osc) const;

    /// Deletes MIDI and event's data for the specified pattern.
    void delete_pattern(PatternId id) const;

    /// Tries to convert the content of a file into a MIDI sequence.
    ///
    /// Will return `std::nullopt` if the file has no valid MIDI data.
    ///
    /// If the MIDI file has data on multiple tracks, all messages will be merged into a single MIDIMessageSequence.
    static std::optional<juce::MidiMessageSequence> load_midi(const juce::File& file);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlarechainAudioProcessor)

    PatternList pattern_list;

    /// Normalizes a MIDI sequence:
    /// - trims any silence at the start
    /// - shifts all events so the first event starts at timestamp 0
    static void normalize_midi(juce::MidiMessageSequence& midi);
};
