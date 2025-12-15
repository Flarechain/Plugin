#include "PlayButton.h"
#include "palettes/Palette.h"

PlayButton::PlayButton() : Button("PLAY")
{
    is_playing = false;
    icon = IconPalette::Play(8, ColorPalette::Coffee500);
    setSize(22, 22);
}

void PlayButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    // button circle
    g.setColour(fill_color);
    g.fillEllipse(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()));

    // icon position. Optical correction: icon centered but moved to right by 1px
    const auto content_bounds = getLocalBounds().translated(1, 0);
    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize,
        1.0f
    );

    icon->setTopLeftPosition(getX() + 10, getY());
}

void PlayButton::play()
{
    is_playing = true;
}