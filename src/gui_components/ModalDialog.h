#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "palettes/Palette.h"
#include "Button.h"

class ModalDialog : public juce::Component
{
public:
    ModalDialog(juce::String title, juce::String subtitle, juce::String primary_action_label, juce::String secondary_action_label);
    ~ModalDialog() override = default;

    void paint(juce::Graphics& g) override;

    std::function<void()> on_primary_action;
    std::function<void()> on_secondary_action;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalDialog)

    juce::String title, subtitle;

    Icon icon;
    juce::TextLayout title_layout, subtitle_layout;
    Button primary_button, secondary_button;
    int title_lines, subtitle_lines;
    static constexpr int PADDING = 16;
    static constexpr int GAP_ICON_TEXT = 12;
    static constexpr int GAP_TEXT = 4;
    static constexpr int GAP_TEXT_BUTTONS = 20;
    static constexpr int MAX_TITLE_LINES = 3;
    static constexpr int MAX_SUBTITLE_LINES = 4;
    static constexpr int TITLE_LINE_SPACING = 2;
};