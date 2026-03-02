#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "PatternListView.h"
#include "TabBar.h"

class PageView : public juce::Component
{
public:
    PageView(const PatternList& pattern_list);
    ~PageView() override = default;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PageView)

    Page current_page;

    TabBar tab_bar;
    PatternListView pattern_list_view;
};

// TODO: for each class in this project, check whether it should be public/private..