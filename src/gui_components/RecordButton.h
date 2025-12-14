#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RecordButton : public juce::Button
{
public:
    RecordButton();

    ~RecordButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void record();
    void stop();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordButton)

    std::unique_ptr<juce::Drawable> icon;
    bool is_recording;
};
