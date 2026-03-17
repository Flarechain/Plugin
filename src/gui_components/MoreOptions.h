#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MoreOptionsMenu.h"
#include "MoreOptionsButton.h"

class MoreOptions : public juce::Component
{
public:
    MoreOptions(int menu_width);
    ~MoreOptions() override = default;

    void add_item(const juce::String& item_text, Icon item_icon, const std::function<void()>& on_click);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptions)

    MoreOptionsButton button;
    MoreOptionsMenu menu;
    static constexpr int GAP = 4;

    void focusLost(FocusChangeType cause) override;
    void open_menu();
    void close_menu();
};