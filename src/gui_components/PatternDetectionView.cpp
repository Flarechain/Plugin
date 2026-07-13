#include "PatternDetectionView.h"
#include "BinaryData.h"

PatternDetectionView::PatternDetectionView()
{
    setInterceptsMouseClicks(false, true);

    const auto font = FontPalette::Display_XXL_Bold;
    const auto text = juce::String("A");
    detection_label.setText(text, juce::dontSendNotification);
    detection_label.setFont(font);
    detection_label.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    detection_label.setSize(76, static_cast<int>(font.getHeight()));
    detection_label.setMinimumHorizontalScale(1.0f); // disable font stretching
    detection_label.setAlpha(0.2f);
    detection_label.setInterceptsMouseClicks(false, true);

    addAndMakeVisible(detection_label);

    setSize(474, ILLUSTRATION_HEIGHT);
}

void PatternDetectionView::paint(juce::Graphics& g)
{
    const juce::Image illustration = juce::ImageFileFormat::loadFrom(BinaryData::chameleon_png, BinaryData::chameleon_pngSize);
    g.drawImage(illustration, getLocalBounds().toFloat(), juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yTop);
}

void PatternDetectionView::resized()
{
    const juce::RectanglePlacement placement { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    detection_label.setBounds(placement.appliedTo(detection_label.getLocalBounds(), getLocalBounds()));
}

void PatternDetectionView::set_detection_label(juce::String label)
{
    detection_label.setAlpha(1.0f);
    detection_label.setText(std::move(label), juce::dontSendNotification);
}
