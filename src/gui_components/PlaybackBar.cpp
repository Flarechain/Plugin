#include "PlaybackControl.h"
#include "palettes/Palette.h"

PlaybackControl::PlaybackBar::PlaybackBar(double &progress) : juce::ProgressBar(progress)
{
    setSize(250, 12);
}

void PlaybackControl::PlaybackBar::paint(juce::Graphics& g)
{
    constexpr int stroke_weight = 2;

    // progress bar stroke
    g.setColour(ColorPalette::Coffee500);
    g.fillRoundedRectangle(
        0,
        0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        static_cast<float>(getHeight() / 2.0)
    );

    // progress bar
    // TODO: dynamic progress bar
    const int inner_bar_width = getWidth() - stroke_weight * 2;
    const int inner_bar_height = getHeight() - stroke_weight * 2;
    const auto fill_color = juce::ColourGradient::horizontal(
        ColorPalette::Orange500,
        0,
        ColorPalette::Red500,
        static_cast<float>(inner_bar_width)
    );
    g.setGradientFill(fill_color);
    g.fillRoundedRectangle(
        stroke_weight,
        stroke_weight,
        static_cast<float>(inner_bar_width),
        static_cast<float>(inner_bar_height),
        static_cast<float>(inner_bar_height / 2.0)
    );
}

void PlaybackControl::PlaybackBar::enablementChanged()
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

void PlaybackControl::PlaybackBar::play()
{

}

void PlaybackControl::PlaybackBar::stop()
{

}