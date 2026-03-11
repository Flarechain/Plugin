#include "PageView.h"

PageView::PageView(const PatternList& pattern_list) : tab_bar({Page::PatternSetup, Page::LiveDetection}), pattern_list_view(pattern_list)
{
    setInterceptsMouseClicks(false, true);

    current_page = tab_bar.get_current_page();
    tab_bar.on_change = [this](Page page)
    {
        current_page = page;
        resized();
    };

    addAndMakeVisible(tab_bar);
    addAndMakeVisible(pattern_list_view);
    setSize(704, 428 + tab_bar.getHeight());
}

void PageView::resized()
{
    juce::RectanglePlacement placement { juce::RectanglePlacement::xMid | juce::RectanglePlacement::yTop | juce::RectanglePlacement::doNotResize };
    auto tab_bar_bounds = placement.appliedTo(tab_bar.getLocalBounds(), getLocalBounds());
    tab_bar.setBounds(tab_bar_bounds);

    auto page_content_bounds = juce::Rectangle<int>(0, tab_bar.getHeight(), getWidth(), getHeight() - tab_bar.getHeight());
    switch (current_page)
    {
        case Page::PatternSetup:
            pattern_list_view.setVisible(true);

            placement = { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
            page_content_bounds = placement.appliedTo(pattern_list_view.getLocalBounds(), page_content_bounds);
            pattern_list_view.setBounds(page_content_bounds);
            break;
        case Page::LiveDetection:
            pattern_list_view.setVisible(false);

            break;
        default:
            break;
    }
}
