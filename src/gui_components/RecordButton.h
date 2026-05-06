#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class RecordButton : public juce::Button
{
public:
    /// Creates a new RecordButton.
    RecordButton();

    ~RecordButton() override = default;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void enablementChanged() override;

    /// Updates the internal recording state and visual representation of this button.
    ///
    /// This method should be called when the processor has confirmed that recording has actually started
    /// (not when the user clicks Record).
    void record();

    /// Updates the internal recording state and visual representation of this button.
    ///
    /// This method should be called when the processor has confirmed that recording has actually stopped
    /// (not when the user clicks Stop).
    void stop();

    /// Called when the user requests to record this pattern.
    ///
    /// This does NOT immediately change the visual or internal recording state.
    /// Instead, it should only notify the processor that the user intends to start recording.
    std::function<void()> on_record;

    /// Called when the user requests to stop recording this pattern.
    ///
    /// This does NOT immediately change the visual or internal recording state.
    /// Instead, it should only notify the processor that the user intends to stop recording.
    std::function<void()> on_stop;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordButton)

    Icon icon;
    bool is_recording;
};
