#include "PlaybackControl.h"
#include "palettes/Palette.h"

PlaybackControl::PlaybackControl() : juce::Component(), playback_bar(playback_progress), playback_progress(0.0)
{
    setInterceptsMouseClicks(false, true);

    addAndMakeVisible(play_button);
    addAndMakeVisible(playback_bar);
    setSize(288, 22);
}

void PlaybackControl::resized()
{
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    fb.alignItems = juce::FlexBox::AlignItems::center;

    fb.items.add(juce::FlexItem(
        static_cast<float>(play_button.getWidth()),
        static_cast<float>(play_button.getHeight()),
        play_button)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(playback_bar.getWidth()),
        static_cast<float>(playback_bar.getHeight()),
        playback_bar)
    );

    fb.performLayout(getLocalBounds().toFloat());
}
