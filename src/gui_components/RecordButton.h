#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class RecordButton : public juce::Button
{
public:
    RecordButton();
    ~RecordButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void record();
    void stop();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordButton)

    Icon icon;
    bool is_recording;
};
