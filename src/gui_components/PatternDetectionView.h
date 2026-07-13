#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class PatternDetectionView : public juce::Component
{
public:
    /// Creates a new PatternDetectionView, composed of an illustration and the label of the detected pattern.
    PatternDetectionView();

    ~PatternDetectionView() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

    /// Changes the detection label with the given one.
    void set_detection_label(juce::String label);

private:
    juce::Label detection_label;

    static constexpr int ILLUSTRATION_HEIGHT = 220;
};