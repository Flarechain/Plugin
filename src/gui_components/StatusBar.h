#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

enum Status
{
    Ok,
    Warning,
    Loading
};

class StatusBar : public juce::Component
{
public:
    StatusBar(Status status, juce::String  message);
    ~StatusBar() override = default;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatusBar)

    Status status;
    juce::String message;

    Icon icon;
};