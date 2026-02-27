#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PatternView.h"
#include "EventView.h"
#include "../model/PatternList.h"

class PatternListView : public juce::Component
{
public:
    PatternListView(const PatternList& pattern_list);

    ~PatternListView() override = default;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternListView)

    // TODO: lock/mutex on patterns and events
    const PatternList& pattern_list;
    std::vector<std::unique_ptr<PatternView>> pattern_views;
    std::vector<std::unique_ptr<EventView>> event_views;
};