#include "Event.h"

Event::Event() : osc_message("/")
{
}

void Event::clear()
{
    ip_address = juce::IPAddress();
    osc_message = juce::OSCMessage("/");
}
