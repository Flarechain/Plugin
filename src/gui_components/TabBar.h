#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ToggleButton.h"
#include "Page.h"

class TabBar : public juce::Component
{
public:
    TabBar(std::vector<Page> pages);
    ~TabBar() override = default;

    void paint(juce::Graphics& g) override;

    Page get_current_page() const;
    std::function<void(Page)> on_selection_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabBar)

    Page current_page;

    std::vector<std::unique_ptr<ToggleButton>> tabs;

    void set_current_page(Page page);
};