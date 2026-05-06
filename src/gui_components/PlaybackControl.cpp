#include "PlaybackControl.h"
#include "palettes/Palette.h"

PlaybackControl::PlaybackControl() : playback_bar(playback_progress), playback_progress(0.0)
{
    setInterceptsMouseClicks(false, true);
    is_playing = false;

    play_button.onClick = [this]()
    {
        if (is_playing)
        {
            if (on_stop) { on_stop(); }
        }
        else
        {
            if (on_play) { on_play(); }
        }
    };

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

void PlaybackControl::set_play_button_enabled(const bool enabled)
{
    if (enabled) { play_button.setEnabled(true); }
    else { play_button.setEnabled(false); }
}

void PlaybackControl::play()
{
    is_playing = true;
    play_button.play();
    playback_bar.play();
}

void PlaybackControl::stop()
{
    is_playing = false;
    play_button.stop();
    playback_bar.stop();
}