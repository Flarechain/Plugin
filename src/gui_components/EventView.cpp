#include "EventView.h"
#include "palettes/Palette.h"
#include "../core/OscParser.h"

EventView::EventView(const Pattern& pattern) : pattern(pattern)
{
    setInterceptsMouseClicks(false, true);

    const auto font = FontPalette::Text_L_Semibold;
    const auto text = juce::String("Event ") + pattern.get_name();
    event_label.setText(text, juce::dontSendNotification);
    event_label.setFont(font);
    event_label.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    event_label.setSize(80, static_cast<int>(font.getHeight()));
    event_label.setMinimumHorizontalScale(1.0f); // disable font stretching
    event_label.setInterceptsMouseClicks(false, true);

    ip_address_field.on_change = [this](std::optional<juce::IPAddress> ip)
    {
        if (on_ip_change) { on_ip_change(ip); }
    };

    text_field.on_change = [this](const juce::String& text)
    {
        if (on_osc_change) { on_osc_change(OscParser::parse(text)); }
    };

    refresh();

    addAndMakeVisible(event_label);
    addAndMakeVisible(ip_address_field);
    addAndMakeVisible(text_field);

    constexpr int gap = 8;
    const int top_row_height = std::max(event_label.getHeight(), ip_address_field.getHeight());
    setSize(text_field.getWidth(), top_row_height + gap + text_field.getHeight());
}

void EventView::resized()
{
    auto top_row_bounds = juce::Rectangle(
            text_field.getWidth(),
            std::max(event_label.getHeight(), ip_address_field.getHeight())
    );

    const juce::RectanglePlacement top_row_placement { juce::RectanglePlacement::yTop | juce::RectanglePlacement::xMid | juce::RectanglePlacement::doNotResize };
    top_row_bounds = top_row_placement.appliedTo(top_row_bounds, getLocalBounds());

    const juce::RectanglePlacement event_label_placement { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    event_label.setBounds(event_label_placement.appliedTo(event_label.getLocalBounds(), top_row_bounds));

    const juce::RectanglePlacement ip_field_placement { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    ip_address_field.setBounds(ip_field_placement.appliedTo(ip_address_field.getLocalBounds(), top_row_bounds));

    const juce::RectanglePlacement text_field_placement { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    text_field.setBounds(text_field_placement.appliedTo(text_field.getLocalBounds(), getLocalBounds()));
}

void EventView::refresh()
{
    if (pattern.has_empty_midi())
    {
        event_label.setAlpha(0.2f);
        ip_address_field.setAlpha(0.2f);
        text_field.setAlpha(0.2f);
        ip_address_field.setEnabled(false);
        text_field.setEnabled(false);
        ip_address_field.setInterceptsMouseClicks(false, false);
        text_field.setInterceptsMouseClicks(false, false);
        text_field.clear();
        ip_address_field.clear();
    }
    else
    {
        event_label.setAlpha(1.0f);
        ip_address_field.setAlpha(1.0f);
        text_field.setAlpha(1.0f);
        ip_address_field.setEnabled(true);
        text_field.setEnabled(true);
        ip_address_field.setInterceptsMouseClicks(false, true);
        text_field.setInterceptsMouseClicks(true, true);

        const auto ip_address = pattern.get_event().get_ip_address();
        const auto osc_message = pattern.get_event().get_osc_message();
        if (ip_address) { ip_address_field.set_ip(ip_address.value()); }
        if (osc_message) { text_field.setText(toString(osc_message.value())); }
    }
    repaint();
}