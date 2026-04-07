#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "palettes/Palette.h"
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

    /// LogPanel subcomponent for displaying a single LogEntry.
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

    const juce::uint8 num_logs;
    std::deque<LogMessage> log_messages;

    std::vector<std::unique_ptr<LogEntry>> logs;
    LinkButton link_button;
    static constexpr int PADDING_X = 8;
    static constexpr int PADDING_Y = 16;
    static constexpr int GAP = 16;
    static constexpr int GAP_LOGS = 2;
};