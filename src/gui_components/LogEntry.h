#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"
#include "LogTypes.h"

class LogEntry : public juce::Component
{
public:
    LogEntry(LogMessage message, int width, bool is_most_recent);
    ~LogEntry() override = default;

    void paint(juce::Graphics& g) override;

    void set_message(LogMessage message);
    static int get_padding_x() { return PADDING_X; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogEntry)

    LogMessage message;
    bool is_most_recent;

    static constexpr int PADDING_X = 16;
    static constexpr int PADDING_Y = 2;
    static constexpr int PADDING_X_HIGHLIGHT = 4;
    static constexpr int PADDING_Y_HIGHLIGHT = 2;
    static constexpr int GAP = 4;
};