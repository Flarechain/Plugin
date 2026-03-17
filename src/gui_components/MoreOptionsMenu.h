#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MoreOptionsItem.h"

class MoreOptionsMenu : public juce::Component
{
public:
    MoreOptionsMenu(int width);
    ~MoreOptionsMenu() override = default;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsMenu)

    int width;
    static constexpr int PADDING = 4;

    std::vector<std::unique_ptr<MoreOptionsItem>> items;

    void add_item(juce::String text, Icon icon, std::function<void()> on_click);

    friend class MoreOptions;
};