#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class MoreOptionsButton : public juce::Button
{
public:
    MoreOptionsButton();

    ~MoreOptionsButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsButton)

    std::unique_ptr<juce::Drawable> icon;
};
