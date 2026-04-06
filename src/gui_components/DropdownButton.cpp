#pragma once

#include "DropdownButton.h"
#include "palettes/Palette.h"

DropdownButton::DropdownButton(const int width, const juce::String& text) : Button(text)
{
    icon = IconPalette::Chevron(8, ColorPalette::Coffee500);

    const auto font = FontPalette::Text_S_Semibold;
    setSize(width, static_cast<int>(std::ceil(font.getHeight())) + PADDING_Y * 2);
}

void DropdownButton::paintButton(juce::Graphics& g, const bool isMouseOverButton, const bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);

    const auto font = FontPalette::Text_S_Semibold;

    auto content_bounds = juce::Rectangle<int>(0,
        0,
        getWidth() - PADDING_X * 2,
        static_cast<int>(std::ceil(font.getHeight()))
    );
    const juce::RectanglePlacement placement { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
    content_bounds = placement.appliedTo(content_bounds, getLocalBounds());

    g.setFont(font);
    g.setColour(ColorPalette::Coffee500);
    g.drawFittedText(getButtonText(), content_bounds, juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid, 1.0f);
    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}