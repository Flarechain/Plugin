#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "IpAddressField.h"
#include "TextField.h"

#include "../model/Pattern.h"

class EventView : public juce::Component
{
public:
    EventView(const Pattern& pattern);
    ~EventView() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EventView)

    const Pattern& pattern;

    juce::Label event_label;
    IpAddressField ip_address_field;
    TextField text_field;
};