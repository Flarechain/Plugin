#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class TextField : public juce::TextEditor
{
public:
    TextField();
    ~TextField() override = default;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextField)
};