#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <objc/objc.h>

FlarechainAudioProcessor::FlarechainAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ),    pattern_list(NUM_PATTERNS), midi_normalizer(MAX_MIDI_DURATION_SECONDS),
            recording_engine(MAX_MIDI_DURATION_SECONDS), inference_engine(NUM_PATTERNS)
{
    selected_instrument = Instrument::Guitar;

    playback_engine.on_play = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::PlaybackStart, id });
        triggerAsyncUpdate();
    };
    playback_engine.on_stop = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::PlaybackStop, id });
        triggerAsyncUpdate();
    };

    recording_engine.on_record = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::RecordingStart, id });
        triggerAsyncUpdate();
    };
    recording_engine.on_stop = [this](const PatternId id, juce::MidiMessageSequence midi)
    {
        set_midi(id, std::move(midi));

        async_event_queue.push(AsyncEvent { AsyncEvent::Type::RecordingStop, id });
        triggerAsyncUpdate();
    };

    inference_engine.on_pattern_detected = [this](const PatternId id)
    {
        async_event_queue.push(AsyncEvent { AsyncEvent::Type::PatternDetected, id });
        triggerAsyncUpdate();
    };

    // load the AI model and start the inference engine
    juce::File model_file;
    #if JUCE_MAC
        model_file = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
            .getParentDirectory()
            .getParentDirectory()
            .getChildFile("Resources/Models/default.onnx");
    #else
        model_file = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
            .getParentDirectory()
            .getChildFile("Models/default.onnx");
    #endif
    inference_engine.start(model_file, 0.8f);

    log_file = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("Flarechain/flarechain.log");
    logger = std::make_unique<juce::FileLogger>(log_file, "Flarechain Log of Detected Patterns");
    juce::Logger::setCurrentLogger(logger.get());
}

FlarechainAudioProcessor::~FlarechainAudioProcessor()
{
    juce::Logger::setCurrentLogger(nullptr);
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
    juce::ignoreUnused (samplesPerBlock);

    playback_engine.set_sample_rate(sampleRate);
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
    if (playback_engine.is_playing())
    {
        midiMessages = playback_engine.get_next_buffer(buffer.getNumSamples());
    }
    else if (recording_engine.is_recording())
    {
        recording_engine.add_buffer(midiMessages, buffer.getNumSamples());
    }
    else if (is_model_downloaded())
    {
        inference_engine.push_buffer(midiMessages);
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
    juce::ValueTree state("FlarechainState");
    state.setProperty("version", JucePlugin_VersionString, nullptr);

    state.appendChild(pattern_list.to_value_tree(), nullptr);
    state.appendChild(to_value_tree(selected_instrument), nullptr);

    juce::MemoryOutputStream stream(destData, false);
    state.writeToStream(stream);
}

void FlarechainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    const auto state = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    pattern_list.from_value_tree(state.getChildWithName("PatternList"));
    const auto instrument_tree = state.getChildWithName("Instrument");
    if (const auto instrument = from_value_tree(instrument_tree))
    {
        selected_instrument = instrument.value();
    }

    async_event_queue.push(AsyncEvent { AsyncEvent::Type::PresetLoaded, 0 });
    triggerAsyncUpdate();
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
        else if (event.type == AsyncEvent::Type::PatternDetected)
        {
            const auto pattern = pattern_list.get(event.pattern_id);
            if (pattern.get_event().get_ip_address() && pattern.get_event().get_osc_message())
            {
                send_osc_message(event.pattern_id);

                juce::Logger::writeToLog("Event " + pattern.get_name()
                    + ": sent OSC message \"" + toString(pattern.get_event().get_osc_message().value())
                    + "\" to " + pattern.get_event().get_ip_address()->toString());

                if (on_pattern_detected) { on_pattern_detected(event.pattern_id); }
            }
        }
        else if (event.type == AsyncEvent::Type::PresetLoaded)
        {
            if (on_preset_loaded) { on_preset_loaded(); }
        }
    }
}

void FlarechainAudioProcessor::set_midi(const PatternId id, juce::MidiMessageSequence midi)
{
    if (playback_engine.is_playing() && playback_engine.get_playing_pattern() == id)
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
    if (playback_engine.is_playing() && playback_engine.get_playing_pattern() == id)
    {
        stop_playing();
    }
    pattern_list.get(id).clear();
}

void FlarechainAudioProcessor::play_pattern(const PatternId id)
{
    playback_engine.play(id, pattern_list.get(id).get_midi());
}

void FlarechainAudioProcessor::stop_playing()
{
    playback_engine.stop();
}

void FlarechainAudioProcessor::record_pattern(const PatternId id)
{
    if (playback_engine.is_playing()) { stop_playing(); }
    recording_engine.record(id);
}

void FlarechainAudioProcessor::stop_recording()
{
    recording_engine.stop();
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

void FlarechainAudioProcessor::send_osc_message(const PatternId id)
{
    const auto event = pattern_list.get(id).get_event();
    if (event.get_ip_address() && event.get_osc_message())
    {
        const auto ip = event.get_ip_address().value().toString();
        osc_sender.connect(ip, 7700);
        osc_sender.sendToIPAddress(
            ip,
            7700,
            event.get_osc_message().value()
        );
    }
}