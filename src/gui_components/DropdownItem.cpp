#include "DropdownItem.h"
#include "palettes/Palette.h"

constexpr int PADDING_X = 8;
constexpr int PADDING_Y = 2;

DropdownItem::DropdownItem(const juce::String& text, int width) : juce::ToggleButton(text)
{
    auto font = FontPalette::Text_S_Medium;
    setSize(width, static_cast<int>(font.getHeight()) + PADDING_Y * 2);
}

void DropdownItem::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100.withAlpha(0.0f);
    if (getToggleState())
    {
        fill_color = ColorPalette::Coffee500;
    }
    else
    {
        if (isButtonDown) { fill_color = ColorPalette::Linen700; }
        else if (isMouseOverButton) { fill_color = ColorPalette::Linen600; }
    }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        4);

    auto text_bounds = getLocalBounds().reduced(PADDING_X, PADDING_Y);

    auto font_color = ColorPalette::Coffee500;
    if (getToggleState())   { font_color = ColorPalette::Linen500; }

    g.setFont(FontPalette::Text_S_Medium);
    g.setColour(font_color);
    g.drawText(getButtonText(), text_bounds, juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid, true);
}
