#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
FlarechainAudioProcessorEditor::FlarechainAudioProcessorEditor (FlarechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), pattern_list_view(p.get_pattern_list()), event_view(p.get_pattern_list().get(0))
{
    juce::ignoreUnused(processorRef);
    addAndMakeVisible(pattern_list_view);
    addAndMakeVisible(text_field);
    addAndMakeVisible(ip_block_field);
    addAndMakeVisible(ip_address_field);
    addAndMakeVisible(event_view);
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
    pattern_list_view.setTopLeftPosition(100,100);
    text_field.setTopLeftPosition(460, 200);
    ip_block_field.setTopLeftPosition(460, 240);
    ip_address_field.setTopLeftPosition(460, 280);
    event_view.setTopLeftPosition(460, 320);
}
