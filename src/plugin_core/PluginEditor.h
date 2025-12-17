#pragma once

#include "PluginProcessor.h"

#include "../gui_components/Components.h"

//==============================================================================
class FlarechainAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit FlarechainAudioProcessorEditor (FlarechainAudioProcessor&);
    ~FlarechainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FlarechainAudioProcessor& processorRef;

    PatternView pattern_view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlarechainAudioProcessorEditor)
};
