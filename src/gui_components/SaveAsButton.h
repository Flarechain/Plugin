#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class SaveAsButton : public juce::Button
{
public:
    /// Creates a new SaveAsButton.
    SaveAsButton();

    ~SaveAsButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaveAsButton)

    Icon icon;
    static constexpr int PADDING = 4;
};