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

    /// Updates the internal playback state and visual representation of this control.
    ///
    /// This method should be called when the processor has confirmed that playback has actually started
    /// (not when the user clicks Play).
    void play();

    /// Updates the internal playback state and visual representation of this control.
    ///
    /// This method should be called when the processor has confirmed that playback has actually stopped
    /// (not when the user clicks Stop).
    void stop();

    /// Called when the user requests to start playback of this pattern.
    ///
    /// This does NOT immediately change the visual or internal playback state.
    /// Instead, it should only notify the processor that the user intends to start playback.
    std::function<void()> on_play;

    /// Called when the user requests to stop playback of this pattern.
    ///
    /// This does NOT immediately change the visual or internal playback state.
    /// Instead, it should only notify the processor that the user intends to stop playback.
    std::function<void()> on_stop;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackControl)

    /// PlaybackControl's subcomponent for displaying the PlayButton.
    class PlayButton : public juce::Button
    {
    public:
        PlayButton();

        ~PlayButton() override = default;

        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
        void enablementChanged() override;

        void play();
        void stop();

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
        void stop();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackBar)
    };

    PlayButton play_button;
    PlaybackBar playback_bar;

    double playback_progress;
    bool is_playing;
};
