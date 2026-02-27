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
    FlarechainAudioProcessor& processorRef;
    PatternListView pattern_list_view;
    TextField text_field;
    IpBlockField ip_block_field;
    IpAddressField ip_address_field;
    EventView event_view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlarechainAudioProcessorEditor)
};
