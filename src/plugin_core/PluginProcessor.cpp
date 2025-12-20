#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr juce::uint8 NUM_PATTERNS = 5;

//==============================================================================
FlarechainAudioProcessor::FlarechainAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ), pattern_list(NUM_PATTERNS)
{
}

FlarechainAudioProcessor::~FlarechainAudioProcessor()
{

}

//==============================================================================
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

//==============================================================================
void FlarechainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
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

void FlarechainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(buffer);

    for (auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        std::cout << message.getDescription();
    }
}

//==============================================================================
bool FlarechainAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FlarechainAudioProcessor::createEditor()
{
    return new FlarechainAudioProcessorEditor (*this);
}

//==============================================================================
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlarechainAudioProcessor();
}
