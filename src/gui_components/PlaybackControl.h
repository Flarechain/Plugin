#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class PlaybackControl : public juce::Component
{
public:
    /// Creates a new PlaybackControl, composed of a PlayButton and a PlaybackBar.
    PlaybackControl();

    ~PlaybackControl() override = default;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackControl)

    /// PlaybackControl's subcomponent for displaying the PlayButton.
    class PlayButton : public juce::Button
    {
    public:
        PlayButton();

        ~PlayButton() override = default;

        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
        void play();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayButton)

        Icon icon;
        bool is_playing;
    };

    /// PlaybackControl's subcomponent for displaying the playback progress bar.
    class PlaybackBar : public juce::ProgressBar
    {
    public:
        explicit PlaybackBar(double& progress);

        ~PlaybackBar() override = default;

        void paint(juce::Graphics& g) override;
        void play();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackBar)

        bool is_playing;
    };

    PlayButton play_button;
    PlaybackBar playback_bar;

    double playback_progress;
};
