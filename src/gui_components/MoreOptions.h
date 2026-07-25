#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MoreOptionsMenu.h"
#include "MoreOptionsButton.h"

class MoreOptions : public juce::Component
{
public:
    /// Creates a new MoreOptions button, which will open a MoreOptionsMenu on button click.
    ///
    /// To add items to the menu, use add_item.
    explicit MoreOptions(int menu_width);

    ~MoreOptions() override = default;

    /// Adds a new item to MoreOptionsMenu, with the given text, icon, and action to do when chosen.
    void add_item(const juce::String& item_text, Icon item_icon, const std::function<void()>& on_click);

    /// Clears all items of MoreOptionsMenu, leaving it empty.
    void clear();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptions)

    MoreOptionsButton button;
    MoreOptionsMenu menu;
    bool menu_open;
    static constexpr int GAP = 4;

    void mouseDown(const juce::MouseEvent& event) override;
    void open_menu();
    void close_menu();
};