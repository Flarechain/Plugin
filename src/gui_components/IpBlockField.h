#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class IpBlockField : public juce::TextEditor
{
public:
    IpBlockField();
    ~IpBlockField() override = default;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IpBlockField)
};