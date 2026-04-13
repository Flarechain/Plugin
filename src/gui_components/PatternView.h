#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "RecordButton.h"
#include "MoreOptions.h"
#include "PlaybackControl.h"

#include "../model/Pattern.h"

typedef std::function<void()> CompletionCallback;

class PatternView : public juce::Component
{
public:
    /// Creates a new PatternView.
    explicit PatternView(const Pattern& pattern);

    ~PatternView() override = default;

    void resized() override;

    /// Updates the component state to reflect the current model and triggers a repaint.
    void refresh();

    std::function<void()> on_import;
    std::function<void()> on_delete;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternView)

    const Pattern& pattern;

    juce::Label pattern_label;
    RecordButton record_button;
    MoreOptions more_options;
    PlaybackControl playback_control;

    void import_midi() const;
    void delete_pattern() const;
};