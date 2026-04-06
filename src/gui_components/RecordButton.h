#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class RecordButton : public juce::Button
{
public:
    /// Creates a new RecordButton.
    RecordButton();

    ~RecordButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

    /// Changes the state of RecordButton, setting `is_recording` to true.
    void record();

    /// Changes the state of RecordButton, setting `is_recording` to false.
    void stop();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordButton)

    Icon icon;
    bool is_recording;
};
