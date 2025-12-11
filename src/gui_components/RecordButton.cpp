#include "RecordButton.h"
#include "palettes/Palette.h"

RecordButton::RecordButton() : Button("RECORD")
{
    icon = IconPalette::Record(6, ColorPalette::Coffee500);
    is_recording = false;
    setSize(86, 24);
}

void RecordButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);

    // icon
    if (is_recording) { icon = IconPalette::Stop(6, ColorPalette::Coffee500); }

    // padding
    constexpr int padding_x = 10;
    constexpr int padding_y = 4;
    const auto bounds = juce::Rectangle(
        padding_x,
        padding_y,
        getWidth() - padding_x * 2,
        getHeight() - padding_y * 2
    );

    // text and icon position
    g.setFont(FontPalette::Text_S_Semibold);
    g.setColour(ColorPalette::Coffee500);
    g.drawFittedText(getButtonText(), bounds, juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid, 1.0f);
    icon->drawWithin(g,
        bounds.toFloat(),
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}