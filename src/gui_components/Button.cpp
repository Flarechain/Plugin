#include "Button.h"
#include "palettes/Palette.h"

Button::Button(const juce::String& text, ButtonType type) : juce::Button(text), type(type)
{
    const auto font = FontPalette::Text_S_Medium;
    setSize(font.getStringWidth(text) + PADDING_X * 2,
        static_cast<int>(std::ceil(font.getHeight())) + PADDING_Y * 2);
}

Button::Button(const juce::String& text, ButtonType type, const int fixed_width) : juce::Button(text), type(type)
{
    const auto font = FontPalette::Text_S_Medium;
    setSize(fixed_width,
        static_cast<int>(std::ceil(font.getHeight())) + PADDING_Y * 2);
}

void Button::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    juce::ignoreUnused(isMouseOverButton, isButtonDown);

    // button fill color
    auto fill_color = ColorPalette::Coffee500;
    if (type == Secondary) { fill_color = ColorPalette::Linen600; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        static_cast<float>(getHeight() / 2.0));

    const auto font = FontPalette::Text_S_Medium;
    auto font_color = ColorPalette::Linen500;
    if (type == Secondary) { font_color = ColorPalette::Coffee500; }
    g.setFont(font);
    g.setColour(font_color);
    g.drawFittedText(getButtonText(), getLocalBounds(), juce::RectanglePlacement::centred, 1.0f);
}