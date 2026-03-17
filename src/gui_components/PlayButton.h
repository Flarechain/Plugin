#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

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