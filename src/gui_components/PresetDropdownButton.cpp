#pragma once

#include "PresetDropdownButton.h"
#include "palettes/Palette.h"

PresetDropdownButton::PresetDropdownButton(const juce::String& text) : Button(text)
{
    icon = IconPalette::Chevron(12, ColorPalette::Coffee500);

    const auto font = FontPalette::Text_XL_Semibold;
    const int width = icon->getWidth() + GAP + font.getStringWidth(text) + PADDING_X * 2;
    const int height = static_cast<int>(font.getHeight()) + PADDING_Y * 2;
    setSize(width, height);
}

void PresetDropdownButton::paintButton(juce::Graphics& g, const bool isMouseOverButton, const bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100.withAlpha(0.0f);
    if (isButtonDown) { fill_color = ColorPalette::Linen200; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen100; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);

    const auto font = FontPalette::Text_XL_Semibold;

    auto content_bounds = juce::Rectangle(0,
        0,
        getWidth() - PADDING_X * 2,
        static_cast<int>(std::ceil(font.getHeight()))
    );
    const juce::RectanglePlacement placement { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
    content_bounds = placement.appliedTo(content_bounds, getLocalBounds());

    const auto text_bounds = content_bounds.withTrimmedLeft(icon->getWidth() + GAP);
    g.setFont(font);
    g.setColour(ColorPalette::Coffee500);
    g.drawText(getButtonText(), text_bounds, juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid, true);
    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}

void PresetDropdownButton::resized()
{
    auto const font = FontPalette::Text_XL_Semibold;
    int const width = (icon->getWidth() + GAP + font.getStringWidth(getButtonText()) + PADDING_X * 2) < MAX_WIDTH ?
        (icon->getWidth() + GAP + font.getStringWidth(getButtonText()) + PADDING_X * 2) : MAX_WIDTH;
    int const height = static_cast<int>(font.getHeight()) + PADDING_Y * 2;
    setSize(width, height);
}

void PresetDropdownButton::setDropdownText(const juce::String& text)
{
    setButtonText(text);
    resized();
}
