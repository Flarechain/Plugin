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

    std::function<void(PatternId id)> on_import;
    std::function<void(PatternId id)> on_delete;

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