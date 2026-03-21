#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"
#include "../gui_components/palettes/Palette.h"

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

    const auto font = FontPalette::Text_S_Medium;
    preset_label.setText("PRESET", juce::dontSendNotification);
    preset_label.setFont(font);
    preset_label.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    preset_label.setSize(70, static_cast<int>(font.getHeight()));
    preset_label.setMinimumHorizontalScale(1.0f); // disable font stretching
    preset_label.setInterceptsMouseClicks(false, true);

    instrument_dropdown.add_item(Instrument::Keyboard, "Keyboard", false);
    instrument_dropdown.add_item(Instrument::Drums, "Drums", false);

    preset_dropdown.on_change = [this](Item<PresetId> preset)
    {
        resized();
    };

    addAndMakeVisible(preset_label);
    addAndMakeVisible(preset_dropdown);
    addAndMakeVisible(save_as_button);
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

    int gap = 4;
    auto preset_bounds = juce::Rectangle<int>(0,
        0,
        160,
        preset_label.getHeight() + gap + preset_dropdown.getHeight());
    juce::RectanglePlacement placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    preset_bounds = placement.appliedTo(preset_bounds, header_bounds);

    placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yTop | juce::RectanglePlacement::doNotResize };
    preset_label.setBounds(placement.appliedTo(preset_label.getLocalBounds(), preset_bounds));

    placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    preset_dropdown.setBounds(placement.appliedTo(preset_dropdown.getLocalBounds(), preset_bounds));

    gap = 16;
    auto save_as_bounds = preset_bounds.withTrimmedLeft(preset_dropdown.getWidth() + gap)
        .withTrimmedTop(preset_bounds.getHeight() - preset_dropdown.getHeight());
    placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    save_as_button.setBounds(placement.appliedTo(save_as_button.getLocalBounds(), save_as_bounds));

    gap = 8;
    auto training_bounds = juce::Rectangle<int>(0,
        0,
        std::max(std::max(instrument_dropdown.getWidth(), train_button.getWidth()), status_bar.getWidth()),
        instrument_dropdown.getHeight() + train_button.getHeight() + status_bar.getHeight() + gap * 2);

    placement = { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
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
