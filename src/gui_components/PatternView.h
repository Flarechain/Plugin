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

    /// Updates the playback state and visual representation of PlaybackControl.
    ///
    /// This method should be called when the processor has confirmed that playback has actually started
    /// (not when the user clicks Play).
    void play() { playback_control.play(); }

    /// Updates the playback state and visual representation of PlaybackControl.
    ///
    /// This method should be called when the processor has confirmed that playback has actually stopped
    /// (not when the user clicks Stop).
    void stop_playing() { playback_control.stop(); }

    std::function<void()> on_import;
    std::function<void()> on_delete;

    /// Called when the user requests to start playback of this pattern.
    ///
    /// This does NOT immediately change the visual or internal playback state.
    /// Instead, it should only notify the processor that the user intends to start playback.
    std::function<void()> on_play;

    /// Called when the user requests to stop playback of this pattern.
    ///
    /// This does NOT immediately change the visual or internal playback state.
    /// Instead, it should only notify the processor that the user intends to stop playback.
    std::function<void()> on_stop_playing;

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