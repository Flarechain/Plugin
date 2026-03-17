#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class MoreOptionsItem : public juce::Button
{
public:
    MoreOptionsItem(const juce::String& text, Icon icon, int width);
    ~MoreOptionsItem() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsItem)

    Icon icon;
    static constexpr int PADDING = 4;
    static constexpr int GAP = 8;

    friend class MoreOptionsMenu;
};