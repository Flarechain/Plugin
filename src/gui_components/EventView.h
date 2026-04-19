#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "IpAddressField.h"
#include "TextField.h"

#include "../model/Pattern.h"

class EventView : public juce::Component
{
public:
    /// Creates a new EventView, displaying the pattern's event name, IP address field, and OSC message text field.
    explicit EventView(const Pattern& pattern);

    ~EventView() override = default;

    void resized() override;

    /// Updates the component state to reflect the current model and triggers a repaint.
    void refresh();

    /// Called when the IP address input changes.
    ///
    /// @param ip An optional IP address:
    /// - `std::nullopt` if the input provided by the user is incomplete
    /// - a valid `juce::IPAddress` if all fields are filled
    std::function<void(std::optional<juce::IPAddress> ip)> on_ip_change;

    /// Called when the OSC message input changes.
    ///
    /// @param osc An optional OSC message:
    /// - `std::nullopt` if the input provided by the user is not valid or incomplete
    /// - a `juce::OSCMessage` if the input is a valid OSC message
    std::function<void(std::optional<juce::OSCMessage> osc)> on_osc_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EventView)

    const Pattern& pattern;

    juce::Label event_label;
    IpAddressField ip_address_field;
    TextField text_field;

    std::optional<juce::OSCMessage> string_to_osc(juce::String string);
};