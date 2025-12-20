#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "RecordButton.h"
#include "MoreOptionsButton.h"
#include "PlaybackControl.h"

#include "../model/Pattern.h"

class PatternView : public juce::Component
{
public:
    PatternView(const Pattern& pattern);

    ~PatternView() override = default;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternView)

    const Pattern& pattern;

    juce::Label pattern_label;
    RecordButton record_button;
    MoreOptionsButton more_options_button;
    PlaybackControl playback_control;
};