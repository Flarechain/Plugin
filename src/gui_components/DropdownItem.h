#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class DropdownItem : public juce::ToggleButton
{
public:
    /// Creates a new DropdownItem.
    DropdownItem(const juce::String& text, int width);

    ~DropdownItem() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropdownItem)

    static constexpr int PADDING_X = 8;
    static constexpr int PADDING_Y = 2;
};