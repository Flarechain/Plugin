#include "PluginProcessor.h"
#include "PluginProcessor.h"

#include <objc/objc.h>

#include "PluginEditor.h"

constexpr juce::uint8 NUM_PATTERNS = 5;

FlarechainAudioProcessor::FlarechainAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ), pattern_list(NUM_PATTERNS), recording(MAX_MIDI_DURATION_SECONDS), midi_normalizer(MAX_MIDI_DURATION_SECONDS)
{
    playback.on_play = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::PlaybackStart, id });
        triggerAsyncUpdate();
    };
    playback.on_stop = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::PlaybackStop, id });
        triggerAsyncUpdate();
    };

    recording.on_record = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::RecordingStart, id });
        triggerAsyncUpdate();
    };
    recording.on_stop = [this](const PatternId id, juce::MidiMessageSequence midi)
    {
        set_midi(id, std::move(midi));

        async_event_queue.push(AsyncEvent { AsyncEvent::Type::RecordingStop, id });
        triggerAsyncUpdate();
    };
}

const juce::String FlarechainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlarechainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FlarechainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FlarechainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FlarechainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlarechainAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FlarechainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlarechainAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String FlarechainAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void FlarechainAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void FlarechainAudioProcessor::prepareToPlay (const double sampleRate, const int samplesPerBlock)
{
    playback.set_sample_rate(sampleRate);
}

void FlarechainAudioProcessor::releaseResources()
{

}

bool FlarechainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void FlarechainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (playback.is_playing())
    {
        midiMessages = playback.get_next_buffer(buffer.getNumSamples());
    }
    else if (recording.is_recording())
    {
        recording.add_buffer(midiMessages, buffer.getNumSamples());
    }
}

bool FlarechainAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FlarechainAudioProcessor::createEditor()
{
    return new FlarechainAudioProcessorEditor (*this);
}

void FlarechainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void FlarechainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlarechainAudioProcessor();
}

void FlarechainAudioProcessor::handleAsyncUpdate()
{
    AsyncEvent event;
    while (async_event_queue.pop(event))
    {
        if (event.type == AsyncEvent::Type::PlaybackStart)
        {
            if (on_playback_start) { on_playback_start(event.pattern_id); }
        }
        else if (event.type == AsyncEvent::Type::PlaybackStop)
        {
            if (on_playback_stop) { on_playback_stop(event.pattern_id); }
        }
        else if (event.type == AsyncEvent::Type::RecordingStart)
        {
            if (on_recording_start) { on_recording_start(event.pattern_id); }
        }
        else if (event.type == AsyncEvent::Type::RecordingStop)
        {
            if (on_recording_stop) { on_recording_stop(event.pattern_id); }
        }
    }
}

void FlarechainAudioProcessor::set_midi(const PatternId id, juce::MidiMessageSequence midi)
{
    if (playback.is_playing() && playback.get_playing_pattern() == id)
    {
        stop_playing();
    }

    midi_normalizer.normalize(midi);
    if (midi.getNumEvents() != 0) { pattern_list.get(id).set_midi(std::move(midi)); }
}

void FlarechainAudioProcessor::set_ip_address(const PatternId id, std::optional<juce::IPAddress> ip) const
{
    pattern_list.get(id).get_event().set_ip_address(ip);
}

void FlarechainAudioProcessor::set_osc_message(const PatternId id, std::optional<juce::OSCMessage> osc) const
{
    pattern_list.get(id).get_event().set_osc_message(std::move(osc));
}

void FlarechainAudioProcessor::delete_pattern(const PatternId id)
{
    if (playback.is_playing() && playback.get_playing_pattern() == id)
    {
        stop_playing();
    }
    pattern_list.get(id).clear();
}

void FlarechainAudioProcessor::play_pattern(const PatternId id)
{
    playback.play(id, pattern_list.get(id).get_midi());
}

void FlarechainAudioProcessor::stop_playing()
{
    playback.stop();
}

void FlarechainAudioProcessor::record_pattern(const PatternId id)
{
    if (playback.is_playing()) { stop_playing(); }
    recording.record(id);
}

void FlarechainAudioProcessor::stop_recording()
{
    recording.stop();
}

std::set<PatternId> FlarechainAudioProcessor::get_active_patterns() const
{
    std::set<PatternId> set;
    for (const auto& pattern : pattern_list)
    {
        if (!pattern->has_empty_midi()) { set.insert(pattern->get_id()); }
    }
    return set;
}

std::set<PatternId> FlarechainAudioProcessor::get_patterns_with_invalid_ip() const
{
    std::set<PatternId> set;
    for (const auto& pattern : pattern_list)
    {
        if (!pattern->has_empty_midi() && !pattern->get_event().get_ip_address()) { set.insert(pattern->get_id()); }
    }
    return set;
}

std::set<PatternId> FlarechainAudioProcessor::get_patterns_with_invalid_osc() const
{
    std::set<PatternId> set;
    for (const auto& pattern : pattern_list)
    {
        if (!pattern->has_empty_midi() && !pattern->get_event().get_osc_message()) { set.insert(pattern->get_id()); }
    }
    return set;
}

std::optional<juce::MidiMessageSequence> FlarechainAudioProcessor::load_midi(const juce::File& file)
{
    juce::FileInputStream stream(file);
    if (!stream.openedOk()) return std::nullopt;

    juce::MidiFile midi_file;
    if (!midi_file.readFrom(stream)) return std::nullopt;
    midi_file.convertTimestampTicksToSeconds();

    juce::MidiMessageSequence midi;
    for (int track = 0; track < midi_file.getNumTracks(); ++track)
    {
        midi.addSequence(*midi_file.getTrack(track), 0.0);
    }

    if (midi.getNumEvents() == 0)   return std::nullopt;
    return midi;
}