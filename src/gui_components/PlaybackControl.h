#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Components.h"
#include "PlaybackBar.h"

class PlaybackControl : public juce::Component
{
public:
    PlaybackControl();

    ~PlaybackControl() override = default;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackControl)

    PlayButton play_button;
    PlaybackBar playback_bar;

    double playback_progress;
};
