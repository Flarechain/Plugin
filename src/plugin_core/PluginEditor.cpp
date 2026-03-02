#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"
#include "../gui_components/palettes/Palette.h"

constexpr int PADDING = 48;

//==============================================================================
FlarechainAudioProcessorEditor::FlarechainAudioProcessorEditor (FlarechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), page_view(p.get_pattern_list())
{
    juce::ignoreUnused(processorRef);

    logo = IconPalette::Logo(20, ColorPalette::Coffee500);

    addAndMakeVisible(page_view);
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

    logo->drawAt(g,
        static_cast<float>(getWidth() / 2) - static_cast<float>(logo->getWidth() / 2),
        PADDING,
        1.0f
    );
}

void FlarechainAudioProcessorEditor::resized()
{
    auto header_bounds = juce::Rectangle<int>(PADDING, PADDING, getWidth() - PADDING * 2, 76);
    auto content_bounds = juce::Rectangle<int>(PADDING,
        getHeight() - PADDING - page_view.getHeight(),
        getWidth() - PADDING * 2,
        page_view.getHeight()
    );

    juce::RectanglePlacement placement = { juce::RectanglePlacement::xMid | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    content_bounds = placement.appliedTo(page_view.getLocalBounds(), content_bounds);
    page_view.setBounds(content_bounds);
}
