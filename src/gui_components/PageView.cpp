#include "PageView.h"

PageView::PageView(const PatternList& pattern_list) :
    tab_bar({Page::PatternSetup, Page::LiveDetection}),
    pattern_list_view(pattern_list),
    log_panel(3, 474)
{
    setInterceptsMouseClicks(false, true);

    current_page = tab_bar.get_current_page();
    tab_bar.on_change = [this](const Page page)
    {
        current_page = page;
        resized();
    };

    pattern_list_view.on_import = [this](const PatternId id)
    {
        if (on_pattern_import) { on_pattern_import(id); }
    };
    pattern_list_view.on_delete = [this](const PatternId id)
    {
        if (on_pattern_delete) { on_pattern_delete(id); }
    };

    addAndMakeVisible(tab_bar);
    addAndMakeVisible(pattern_list_view);
    addAndMakeVisible(log_panel);
    setSize(704, 428 + tab_bar.getHeight());
}

void PageView::resized()
{
    juce::RectanglePlacement placement { juce::RectanglePlacement::xMid | juce::RectanglePlacement::yTop | juce::RectanglePlacement::doNotResize };
    const auto tab_bar_bounds = placement.appliedTo(tab_bar.getLocalBounds(), getLocalBounds());
    tab_bar.setBounds(tab_bar_bounds);

    auto page_content_bounds = juce::Rectangle<int>(0, tab_bar.getHeight(), getWidth(), getHeight() - tab_bar.getHeight());
    switch (current_page)
    {
        case Page::PatternSetup:
            pattern_list_view.setVisible(true);
            log_panel.setVisible(false);

            placement = { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
            page_content_bounds = placement.appliedTo(pattern_list_view.getLocalBounds(), page_content_bounds);
            pattern_list_view.setBounds(page_content_bounds);
            break;
        case Page::LiveDetection:
            log_panel.setVisible(true);
            pattern_list_view.setVisible(false);

            placement = { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
            page_content_bounds = placement.appliedTo(log_panel.getLocalBounds(), page_content_bounds);
            log_panel.setBounds(page_content_bounds);
            break;
        default:
            break;
    }
}
