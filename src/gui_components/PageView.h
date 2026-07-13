#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "TabBar.h"
#include "PatternListView.h"
#include "LogPanel.h"
#include "PatternDetectionView.h"

class PageView : public juce::Component
{
public:
    /// Creates a new PageView with a TabBar to navigate between pages.
    explicit PageView(const PatternList& pattern_list);

    ~PageView() override = default;

    void resized() override;

    /// Updates the component state to reflect the current model and triggers a repaint.
    void refresh() const;

    PatternListView& get_pattern_list_view() { return pattern_list_view; }
    LogPanel& get_log_panel() { return log_panel; }
    PatternDetectionView& get_detection_view() { return detection_view; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PageView)

    Page current_page;

    TabBar tab_bar;
    PatternListView pattern_list_view;
    PatternDetectionView detection_view;
    LogPanel log_panel;
};
