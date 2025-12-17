#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
FlarechainAudioProcessorEditor::FlarechainAudioProcessorEditor (FlarechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    addAndMakeVisible(pattern_view);

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
    pattern_view.setTopLeftPosition(100, 160);
}
