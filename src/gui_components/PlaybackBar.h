#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

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