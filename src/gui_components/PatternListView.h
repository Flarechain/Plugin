#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PatternView.h"
#include "EventView.h"
#include "../model/PatternList.h"

class PatternListView : public juce::Component
{
public:
    /// Creates a new PatternListView, displaying a list of PatternViews.
    explicit PatternListView(const PatternList& pattern_list);

    ~PatternListView() override = default;

    void resized() override;

    /// Updates the component state to reflect the current model and triggers a repaint.
    void refresh() const;

    std::function<void(PatternId id)> on_import;
    std::function<void(PatternId id)> on_delete;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternListView)

    // TODO: lock/mutex on patterns and events
    const PatternList& pattern_list;
    std::vector<std::unique_ptr<PatternView>> pattern_views;
    std::vector<std::unique_ptr<EventView>> event_views;
};