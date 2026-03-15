#include "ToggleButton.h"
#include "palettes/Palette.h"

ToggleButton::ToggleButton(const juce::String& text) : juce::ToggleButton(text)
{
    auto font = FontPalette::Text_S_Medium;
    setSize(PADDING_X * 2 + font.getStringWidth(getButtonText()), PADDING_Y * 2 + static_cast<int>(font.getHeight()));
}

void ToggleButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100.withAlpha(0.0f);
    if (getToggleState())
    {
        fill_color = ColorPalette::Coffee500;
    }
    else
    {
        if (isButtonDown) { fill_color = ColorPalette::Linen200; }
        else if (isMouseOverButton) { fill_color = ColorPalette::Linen100; }
    }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        static_cast<float>(getHeight() / 2));

    auto font_color = ColorPalette::Coffee500;
    if (getToggleState()) { font_color = ColorPalette::Linen500; }

    g.setFont(FontPalette::Text_S_Medium);
    g.setColour(font_color);
    g.drawFittedText(getButtonText(), getLocalBounds(), juce::RectanglePlacement::centred, 1.0f);
}
