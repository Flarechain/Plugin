#include "PatternListView.h"

constexpr int GAP_PATTERNS = 20;     // gap between `PatternView`s
constexpr int GAP_PATTERN_EVENT = 80;       // gap between `PatternView` and `EventView`
constexpr int MAX_VISIBLE_PATTERNS = 5;     // maximum number of `PatternView`s that can be seen without scrolling

PatternListView::PatternListView(const PatternList& pattern_list) : pattern_list(pattern_list)
{
    setInterceptsMouseClicks(false, true);

    for (const auto& pattern : pattern_list)
    {
        auto pattern_view = std::make_unique<PatternView>(*pattern);
        auto event_view = std::make_unique<EventView>(*pattern);

        pattern_view->on_import = [this, &pattern]()
        {
            if (on_import) { on_import(pattern->get_id()); }
        };
        pattern_view->on_delete = [this, &pattern]()
        {
            if (on_delete) { on_delete(pattern->get_id()); }
        };

        addAndMakeVisible(*pattern_view);
        addAndMakeVisible(*event_view);
        pattern_views.push_back(std::move(pattern_view));
        event_views.push_back(std::move(event_view));
    }

    const int width = pattern_views.at(0)->getWidth() + GAP_PATTERN_EVENT + event_views.at(0)->getWidth();
    const int height = std::max(pattern_views.at(0)->getHeight(), event_views.at(0)->getHeight()) * MAX_VISIBLE_PATTERNS + GAP_PATTERNS * (MAX_VISIBLE_PATTERNS - 1);
    setSize(width, height);
}

void PatternListView::resized()
{
    // no flexbox with "justify space between": the number of patterns is variable, but gaps are fixed
    const int height = std::max(pattern_views.at(0)->getHeight(), event_views.at(0)->getHeight());
    int y = 0;
    for (unsigned long i = 0; i < pattern_views.size(); i++)
    {
        constexpr int x = 0;
        const auto &pattern_view = pattern_views.at(i);
        const auto &event_view = event_views.at(i);
        auto bounds = juce::Rectangle(
            x, y,
            pattern_view->getWidth() + GAP_PATTERN_EVENT + event_view->getWidth(),
            std::max(pattern_view->getHeight(), event_view->getHeight())
        );

        juce::RectanglePlacement placement { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
        const auto pattern_bounds = placement.appliedTo(pattern_view->getLocalBounds(), bounds);
        pattern_view->setBounds(pattern_bounds);

        placement = juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize;
        const auto event_bounds = placement.appliedTo(event_view->getLocalBounds(), bounds);
        event_view->setBounds(event_bounds);

        y += height + GAP_PATTERNS;
    }
}
