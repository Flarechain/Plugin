#pragma once

#include <juce_osc/juce_osc.h>

class Event
{
public:
    Event() {}
    ~Event() = default;

    [[nodiscard]] std::optional<juce::OSCMessage> get_osc_message() const { return osc_message; }
    [[nodiscard]] std::optional<juce::IPAddress> get_ip_address() const { return ip_address; }
    void set_osc_message(std::optional<juce::OSCMessage> osc_message) { this->osc_message = osc_message; }
    void set_ip_address(std::optional<juce::IPAddress> ip_address) { this->ip_address = ip_address; }

    /// Resets the Event by clearing its IP address and OSC message.
    void clear();

private:
    std::optional<juce::OSCMessage> osc_message;
    std::optional<juce::IPAddress> ip_address;
};