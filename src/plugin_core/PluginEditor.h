#pragma once

#include "PluginProcessor.h"

#include "../gui_components/Components.h"
#include "../model/Instrument.h"

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

    FlarechainAudioProcessor& processorRef;

    std::unique_ptr<juce::Drawable> logo;
    Dropdown<Instrument> instrument_dropdown;
    Button train_button;
    StatusBar status_bar;
    PageView page_view;
};
