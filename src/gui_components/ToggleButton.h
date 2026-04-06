#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ToggleButton : public juce::ToggleButton
{
public:
    /// Creates a new ToggleButton, used in TabBar.
    explicit ToggleButton(const juce::String& text);

    ~ToggleButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToggleButton)

    static constexpr int PADDING_X = 16;
    static constexpr int PADDING_Y = 2;
};