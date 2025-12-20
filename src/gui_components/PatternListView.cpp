#include "PatternListView.h"

constexpr int GAP = 20;     // gap between `PatternView`s
constexpr int MAX_VISIBLE_PATTERNS = 5;     // maximum number of `PatternView`s that can be seen without scrolling

PatternListView::PatternListView(const PatternList& pattern_list) : pattern_list(pattern_list)
{
    for (const auto& pattern : pattern_list)
    {
        auto pattern_view = std::make_unique<PatternView>(*pattern);
        addAndMakeVisible(*pattern_view);
        pattern_views.push_back(std::move(pattern_view));
    }

    const int width = pattern_views.at(0)->getWidth();
    int height = pattern_views.at(0)->getHeight() * MAX_VISIBLE_PATTERNS + GAP * (MAX_VISIBLE_PATTERNS - 1);
    setSize(width, height);
}

void PatternListView::resized()
{
    const int height = pattern_views.at(0)->getHeight();
    constexpr int x = 0;
    int y = 0;
    for (auto& pattern_view : pattern_views)
    {
        pattern_view->setTopLeftPosition(x, y);
        y += height + GAP;
    }
}
