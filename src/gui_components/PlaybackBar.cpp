#include "PlaybackBar.h"
#include "palettes/Palette.h"

PlaybackBar::PlaybackBar(double &progress) : juce::ProgressBar(progress)
{
    is_playing = false;
    setSize(250, 12);
}

void PlaybackBar::paint(juce::Graphics& g)
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

void PlaybackBar::play()
{
    is_playing = true;
}
