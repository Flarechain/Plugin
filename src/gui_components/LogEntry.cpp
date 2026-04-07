#include <juce_gui_basics/juce_gui_basics.h>
#include "LogPanel.h"

LogPanel::LogEntry::LogEntry(LogMessage message, const int width, const bool is_most_recent) : message(std::move(message)), is_most_recent(is_most_recent)
{
    const auto font = FontPalette::Text_S_Medium;
    setSize(width, static_cast<int>(font.getHeight()) + PADDING_Y_HIGHLIGHT * 2 + PADDING_Y * 2);
}

void LogPanel::LogEntry::paint(juce::Graphics& g)
{
    const auto fill_color = this->is_most_recent ? ColorPalette::Linen200 : ColorPalette::Linen100.withAlpha(0.0f);
    auto font = FontPalette::Text_S_Medium;

    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);

    float x = PADDING_X;
    float y = 0;
    float text_width = 0;
    float text_height = 0;

    for (const LogText& text : message)
    {
        if (text.is_highlighted())
        {
            font = FontPalette::Text_XS_Medium;
            text_width = font.getStringWidthFloat(text.get_text()) + PADDING_X_HIGHLIGHT * 2;
            text_height = font.getHeight() + PADDING_Y_HIGHLIGHT * 2;
        }
        else
        {
            font = FontPalette::Text_S_Medium;
            text_width = font.getStringWidthFloat(text.get_text());
            text_height = font.getHeight();
        }

        y = (static_cast<float>(getHeight()) - text_height) / 2;
        const auto bounds = juce::Rectangle(x, y, text_width, text_height);

        if (text.is_highlighted())
        {
            const auto highlight_color = this->is_most_recent ? ColorPalette::Linen300 : ColorPalette::Linen200;
            g.setColour(highlight_color);
            g.fillRoundedRectangle(bounds, 4);
        }

        g.setColour(ColorPalette::Coffee500);
        g.setFont(font);
        g.drawText(
            text.get_text(),
            bounds,
            juce::Justification::centred,
            false
        );

        x += text_width + GAP;
    }
}

void LogPanel::LogEntry::set_message(LogMessage message)
{
    this->message = std::move(message);
    repaint();
}
