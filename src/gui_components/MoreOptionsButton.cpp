#include "MoreOptionsButton.h"
#include "palettes/Palette.h"

MoreOptionsButton::MoreOptionsButton() : Button("MORE OPTIONS")
{
    icon = IconPalette::MoreOptions(16, ColorPalette::Coffee500);
    setSize(24, 24);
}

void MoreOptionsButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    // button circle
    g.setColour(fill_color);
    g.fillEllipse(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()));

    // icon position
    const auto content_bounds = getLocalBounds();
    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}