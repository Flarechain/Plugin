#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class LinkButton : public juce::Button
{
public:
    LinkButton(juce::String text);
    ~LinkButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinkButton)

    Icon icon;
    static constexpr int PADDING_X = 8;
    static constexpr int PADDING_Y = 4;
    static constexpr int GAP = 8;
};