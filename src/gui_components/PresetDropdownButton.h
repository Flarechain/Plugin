#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class PresetDropdownButton : public juce::Button
{
public:
    PresetDropdownButton(juce::String text);
    ~PresetDropdownButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void resized() override;
    void setDropdownText(juce::String text);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetDropdownButton)

    // setButtonText() is only allowed inside this class, not on PresetDropdownButton API
    // To change buttonText, use setDropdownText() instead
    using juce::Button::setButtonText;

    Icon icon;
    static constexpr int PADDING_X = 8;
    static constexpr int PADDING_Y = 2;
    static constexpr int GAP = 8;
    static constexpr int MAX_WIDTH = 154;
};