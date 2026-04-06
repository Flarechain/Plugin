#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MoreOptionsItem.h"

class MoreOptionsMenu : public juce::Component
{
public:
    /// Creates a new MoreOptionsMenu.
    explicit MoreOptionsMenu(int width);

    ~MoreOptionsMenu() override = default;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsMenu)

    int width;
    static constexpr int PADDING = 4;

    std::vector<std::unique_ptr<MoreOptionsItem>> items;

    /// Adds a new item to the menu with the given text, icon, and action to do when chosen.
    void add_item(const juce::String& text, Icon icon, std::function<void()> on_click);

    friend class MoreOptions;
};