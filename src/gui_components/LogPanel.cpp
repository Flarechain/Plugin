#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "LogPanel.h"

LogPanel::LogPanel(juce::uint8 num_logs, const int width) : num_logs(num_logs), link_button("Open log file")
{
    if (num_logs == 0) { num_logs = 1; }

    auto log_entry_width = width - PADDING_X * 2;

    auto log_entry = std::make_unique<LogEntry>(LogText(""), log_entry_width, true);
    logs.push_back(std::move(log_entry));
    addChildComponent(*logs.back());

    for (int i = 2; i <= num_logs; i++)
    {
        log_entry = std::make_unique<LogEntry>(LogText(""), log_entry_width, false);
        logs.push_back(std::move(log_entry));
        addAndMakeVisible(*logs.back());
    }

    link_button.onClick = [this]()
    {
        if (on_open_file) { on_open_file(); }
    };

    addAndMakeVisible(link_button);
    const auto font = FontPalette::Text_L_Semibold;
    setSize(width, static_cast<int>(font.getHeight()) + GAP + logs.at(0)->getHeight() * num_logs + GAP_LOGS * (num_logs - 1) + PADDING_Y * 2);
}

void LogPanel::paint(juce::Graphics& g)
{
    g.setColour(ColorPalette::Linen100);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 16);

    const auto font = FontPalette::Text_L_Semibold;
    const auto label_bounds = juce::Rectangle<int>(PADDING_X + LogEntry::get_padding_x(),
        PADDING_Y,
        getWidth() - PADDING_X * 2 - LogEntry::get_padding_x() * 2,
        static_cast<int>(font.getHeight()));

    g.setColour(ColorPalette::Coffee500);
    g.setFont(font);
    g.drawText("Log",
        label_bounds,
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid,
        1.0f);

    const juce::RectanglePlacement placement { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    const auto button_bounds = placement.appliedTo(link_button.getLocalBounds(), label_bounds);
    link_button.setBounds(button_bounds);

    constexpr int x = PADDING_X;
    int y = label_bounds.getBottom() + GAP;
    for (const auto& log : logs)
    {
        log->setTopLeftPosition(x, y);
        y += log->getHeight() + GAP_LOGS;
    }
}

void LogPanel::add_log(LogMessage message)
{
    if (!logs.at(0)->isVisible()) { logs.at(0)->setVisible(true); }

    log_messages.push_front(std::move(message));
    if (log_messages.size() > num_logs) { log_messages.pop_back(); }

    for (juce::uint8 i = 0; i < log_messages.size(); i++)
    {
        logs.at(i)->set_message(log_messages.at(i));
    }
}
