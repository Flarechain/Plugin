#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"
#include "../gui_components/palettes/Palette.h"

constexpr int PADDING = 48;

//==============================================================================
FlarechainAudioProcessorEditor::FlarechainAudioProcessorEditor (FlarechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p),
    instrument_dropdown(90, Instrument::Guitar, "Guitar"),
    train_button("Train model", Primary),
    status_bar(Warning, "Model not trained"),
    page_view(p.get_pattern_list())
{
    juce::ignoreUnused(processorRef);
    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);

    logo = IconPalette::Logo(20, ColorPalette::Coffee500);

    instrument_dropdown.add_item(Instrument::Keyboard, "Keyboard");
    instrument_dropdown.add_item(Instrument::Drums, "Drums");

    addAndMakeVisible(instrument_dropdown);
    addAndMakeVisible(train_button);
    addAndMakeVisible(status_bar);
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
    // header section
    auto header_bounds = juce::Rectangle<int>(PADDING, PADDING, getWidth() - PADDING * 2, 76);

    constexpr int gap = 8;
    auto training_bounds = juce::Rectangle<int>(0,
        0,
        std::max(std::max(instrument_dropdown.getWidth(), train_button.getWidth()), status_bar.getWidth()),
        instrument_dropdown.getHeight() + train_button.getHeight() + status_bar.getHeight() + gap * 2);

    juce::RectanglePlacement placement = { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    training_bounds = placement.appliedTo(training_bounds, header_bounds);

    juce::FlexBox training_flex;
    training_flex.flexDirection = juce::FlexBox::Direction::column;
    training_flex.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    training_flex.alignItems = juce::FlexBox::AlignItems::flexEnd;

    training_flex.items.add(juce::FlexItem(
        static_cast<float>(instrument_dropdown.getWidth()),
        static_cast<float>(instrument_dropdown.getHeight()),
        instrument_dropdown)
    );

    training_flex.items.add(juce::FlexItem(
        static_cast<float>(train_button.getWidth()),
        static_cast<float>(train_button.getHeight()),
        train_button)
    );

    training_flex.items.add(juce::FlexItem(
        static_cast<float>(status_bar.getWidth()),
        static_cast<float>(status_bar.getHeight()),
        status_bar)
    );
    training_flex.performLayout(training_bounds.toFloat());

    // content section
    auto content_bounds = juce::Rectangle<int>(PADDING,
        getHeight() - PADDING - page_view.getHeight(),
        getWidth() - PADDING * 2,
        page_view.getHeight()
    );

    placement = { juce::RectanglePlacement::xMid | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    content_bounds = placement.appliedTo(page_view.getLocalBounds(), content_bounds);
    page_view.setBounds(content_bounds);
}
