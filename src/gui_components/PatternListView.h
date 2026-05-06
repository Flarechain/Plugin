#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PatternView.h"
#include "EventView.h"
#include "../model/PatternList.h"

class PatternListView : public juce::Component
{
public:
    /// Creates a new PatternListView, displaying a list of PatternViews with the related EventViews.
    explicit PatternListView(const PatternList& pattern_list);

    ~PatternListView() override = default;

    void resized() override;

    /// Updates the component state to reflect the current model and triggers a repaint.
    void refresh() const;

    /// Updates the playback state and visual representation of PlaybackControl of the specified pattern.
    ///
    /// This method should be called when the processor has confirmed that playback has actually started
    /// (not when the user clicks Play).
    void play(const PatternId id) const { pattern_views[id]->play(); }

    /// Updates the playback state and visual representation of PlaybackControl of the specified pattern.
    ///
    /// This method should be called when the processor has confirmed that playback has actually stopped
    /// (not when the user clicks Stop).
    void stop_playing(const PatternId id) const { pattern_views[id]->stop_playing(); }

    /// Updates the recording state and visual representation of RecordButton of the specified pattern.
    ///
    /// This method should be called when the processor has confirmed that recording has actually started
    /// (not when the user clicks Record).
    void record(PatternId id) const;

    /// Updates the recording state and visual representation of RecordButton of the specified pattern.
    ///
    /// This method should be called when the processor has confirmed that recording has actually stopped
    /// (not when the user clicks Stop).
    void stop_recording(PatternId id) const;

    std::function<void(PatternId id)> on_import;
    std::function<void(PatternId id)> on_delete;

    /// Called when the user requests to start playback of a specific pattern.
    ///
    /// This does NOT immediately change the visual or internal playback state.
    /// Instead, it should only notify the processor that the user intends to start playback.
    std::function<void(PatternId id)> on_play;

    /// Called when the user requests to stop playback of a specific pattern.
    ///
    /// This does NOT immediately change the visual or internal playback state.
    /// Instead, it should only notify the processor that the user intends to stop playback.
    std::function<void(PatternId id)> on_stop_playing;

    /// Called when the user requests to record a specific pattern.
    ///
    /// This does NOT immediately change the visual or internal recording state.
    /// Instead, it should only notify the processor that the user intends to start recording.
    std::function<void(PatternId id)> on_record;

    /// Called when the user requests to stop recording a specific pattern.
    ///
    /// This does NOT immediately change the visual or internal recording state.
    /// Instead, it should only notify the processor that the user intends to stop recording.
    std::function<void(PatternId id)> on_stop_recording;

    /// Called when a pattern's IP address input changes.
    ///
    /// @param ip An optional IP address:
    /// - `std::nullopt` if the input provided by the user is incomplete
    /// - a valid `juce::IPAddress` if all fields are filled
    std::function<void(PatternId id, std::optional<juce::IPAddress> ip)> on_ip_change;

    /// Called when a pattern's OSC message input changes.
    ///
    /// @param osc An optional OSC message:
    /// - `std::nullopt` if the input provided by the user is not valid
    /// - a `juce::OSCMessage` if the input is a valid OSC message
    std::function<void(PatternId id, std::optional<juce::OSCMessage> osc)> on_osc_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternListView)

    const PatternList& pattern_list;
    std::vector<std::unique_ptr<PatternView>> pattern_views;
    std::vector<std::unique_ptr<EventView>> event_views;
};