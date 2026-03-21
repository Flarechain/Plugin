#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DropdownMenu.h"
#include "DropdownButton.h"
#include "DropdownTypes.tpp"

template<typename T>
class Dropdown : public juce::Component
{
public:
    Dropdown(int width, T default_item_id, juce::String default_item_text);
    ~Dropdown() override = default;

    void add_item(T item_id, juce::String item_text, bool selected);
    T get_selected_item() const;
    std::function<void(Item<T>)> on_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dropdown)

    Item<T> selected_item;

    DropdownButton button;
    DropdownMenu<T> menu;
    static constexpr int GAP = 4;

    void focusLost(FocusChangeType cause) override;
    void open_menu();
    void close_menu();
};

#include "Dropdown.tpp"