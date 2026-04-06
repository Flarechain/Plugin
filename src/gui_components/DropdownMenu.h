#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DropdownItem.h"
#include "DropdownTypes.tpp"

template<typename T>
class Dropdown;

template<typename T>
class DropdownMenu : public juce::Component
{
public:
    /// Creates a new DropdownMenu with the given width and default item.
    DropdownMenu(int width, T default_item_id, const juce::String& default_item_text);

    ~DropdownMenu() override = default;

    void paint(juce::Graphics& g) override;

    void add_item(T id, juce::String text, bool selected);
    T get_selected_item() const;
    std::function<void(Item<T>)> on_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropdownMenu)

    Item<T> selected_item;
    int width;
    static constexpr int PADDING = 4;

    std::vector<std::unique_ptr<DropdownItem>> items;

    void set_selected_item(Item<T> item);
    void clear();

    template<typename U>
    friend class Dropdown;
    friend class PresetDropdown;
};

#include "DropdownMenu.tpp"