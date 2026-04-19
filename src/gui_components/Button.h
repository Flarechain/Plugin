#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

enum ButtonType
{
    Primary,
    Secondary
};

class Button : public juce::Button
{
public:
    /// Creates a new Button with dynamic width.
    Button(const juce::String& text, ButtonType type);

    /// Creates a new Button with fixed width.
    Button(const juce::String& text, ButtonType type, int fixed_width);

    ~Button() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void enablementChanged() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)

    ButtonType type;
    static constexpr int PADDING_X = 16;
    static constexpr int PADDING_Y = 6;
};