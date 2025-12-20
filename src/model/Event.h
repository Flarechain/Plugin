#pragma once

#include <juce_osc/juce_osc.h>

class Event
{
public:
    Event();
    ~Event() = default;

private:
    juce::OSCMessage osc_message;
    juce::IPAddress ip_address;
};