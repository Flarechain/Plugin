#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
FlarechainAudioProcessorEditor::FlarechainAudioProcessorEditor (FlarechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    addAndMakeVisible(record_button);
    addAndMakeVisible(play_button);
    addAndMakeVisible(playback_control);
    addAndMakeVisible(more_options_button);
    record_button.record();

    juce::ignoreUnused(processorRef);
    setSize(800, 600);
}

FlarechainAudioProcessorEditor::~FlarechainAudioProcessorEditor()
{
}

//==============================================================================
void FlarechainAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Image backgroundImage = juce::ImageFileFormat::loadFrom(BinaryData::background_png, BinaryData::background_pngSize);

    g.drawImage(backgroundImage, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
}

void FlarechainAudioProcessorEditor::resized()
{
    record_button.setTopLeftPosition(100, 160);
    play_button.setTopLeftPosition(100, 240);
    playback_control.setTopLeftPosition(100, 320);
    more_options_button.setTopLeftPosition(100, 400);
}
