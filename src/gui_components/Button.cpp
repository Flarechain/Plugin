#include "Button.h"
#include "palettes/Palette.h"

Button::Button(juce::String text, ButtonType type) : juce::Button(text), type(type)
{
    auto font = FontPalette::Text_S_Medium;
    setSize(font.getStringWidth(text) + PADDING_X * 2,
        font.getHeight() + PADDING_Y * 2);
}

Button::Button(juce::String text, ButtonType type, int fixed_width) : juce::Button(text), type(type)
{
    auto font = FontPalette::Text_S_Medium;
    setSize(fixed_width,
        font.getHeight() + PADDING_Y * 2);
}

void Button::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Coffee500;
    if (type == Secondary) { fill_color = ColorPalette::Linen600; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        static_cast<float>(getHeight() / 2));

    auto font = FontPalette::Text_S_Medium;
    auto font_color = ColorPalette::Linen500;
    if (type == Secondary) { font_color = ColorPalette::Coffee500; }
    g.setFont(font);
    g.setColour(font_color);
    g.drawFittedText(getButtonText(), getLocalBounds(), juce::RectanglePlacement::centred, 1.0f);
}