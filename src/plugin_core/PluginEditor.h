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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlarechainAudioProcessorEditor)

    std::unique_ptr<juce::Drawable> logo;
    PageView page_view;

    FlarechainAudioProcessor& processorRef;
};
