#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "palettes/Palette.h"
#include "LogEntry.h"
#include "LogTypes.h"
#include "LinkButton.h"

class LogPanel : public juce::Component
{
public:
    /// Creates a new LogPanel.
    LogPanel(juce::uint8 num_logs, int width);

    ~LogPanel() override = default;

    void paint(juce::Graphics& g) override;

    /// Adds a new log to the LogPanel with the given LogMessage.
    void add_log(LogMessage message);

    std::function<void()> on_open_file;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogPanel)

    const juce::uint8 num_logs;
    std::deque<LogMessage> log_messages;

    std::vector<std::unique_ptr<LogEntry>> logs;
    LinkButton link_button;
    static constexpr int PADDING_X = 8;
    static constexpr int PADDING_Y = 16;
    static constexpr int GAP = 16;
    static constexpr int GAP_LOGS = 2;
};