#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class DropdownButton : public juce::Button
{
public:
    DropdownButton(int width, juce::String text);
    ~DropdownButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropdownButton)

    Icon icon;
    static constexpr int PADDING_X = 8;
    static constexpr int PADDING_Y = 2;
};
