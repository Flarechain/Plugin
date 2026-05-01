#include "PlaybackControl.h"
#include "palettes/Palette.h"

PlaybackControl::PlayButton::PlayButton() : Button("PLAY")
{
    is_playing = false;
    icon = IconPalette::Play(8, ColorPalette::Coffee500);

    setSize(22, 22);
}

void PlaybackControl::PlayButton::paintButton(juce::Graphics& g, const bool isMouseOverButton, const bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    // button circle
    g.setColour(fill_color);
    g.fillEllipse(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()));

    // icon position
    auto content_bounds = getLocalBounds();

    // Optical correction: play icon centered but moved to right by 1px
    if (!is_playing) { content_bounds = content_bounds.translated(1, 0); }

    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}

void PlaybackControl::PlayButton::enablementChanged()
{
    if (isEnabled())
    {
        setAlpha(1.0f);
        setInterceptsMouseClicks(true, true);
    }
    else
    {
        setAlpha(0.2f);
        setInterceptsMouseClicks(false, false);
    }
    repaint();
}

void PlaybackControl::PlayButton::play()
{
    is_playing = true;
    icon = IconPalette::Stop(6, ColorPalette::Coffee500);
    repaint();
}

void PlaybackControl::PlayButton::stop()
{
    is_playing = false;
    icon = IconPalette::Play(8, ColorPalette::Coffee500);
    repaint();
}
