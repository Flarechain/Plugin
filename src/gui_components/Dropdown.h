#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DropdownMenu.h"
#include "DropdownButton.h"
#include "DropdownTypes.tpp"

template<typename T>
class Dropdown : public juce::Component
{
public:
    /// Creates a new Dropdown with the given width and default selected item.
    ///
    /// To add items to the menu, use add_item().
    Dropdown(int width, T default_item_id, juce::String default_item_text);

    ~Dropdown() override = default;

    /// Adds a new item to the menu, specifying if it should be selected.
    ///
    /// Since only one item can be the selected one, the last one added with `selected = true` will become
    /// the selected item.
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