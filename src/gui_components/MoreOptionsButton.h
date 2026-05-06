#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class MoreOptionsButton : public juce::Button
{
public:
    /// Creates a new MoreOptionsButton.
    MoreOptionsButton();

    ~MoreOptionsButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void enablementChanged() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsButton)

    Icon icon;
};
