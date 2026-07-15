#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
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
    void set_selected_item(Item<T> item);
    T get_selected_item() const;

    std::function<void(Item<T>)> on_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropdownMenu)

    /// DropdownMenu's subcomponent for displaying a single item.
    class DropdownItem : public juce::ToggleButton
    {
    public:
        DropdownItem(Item<T> item, int width);

        ~DropdownItem() override = default;

        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

        Item<T> get_item() { return item; }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropdownItem)

        Item<T> item;

        static constexpr int PADDING_X = 8;
        static constexpr int PADDING_Y = 2;
    };

    Item<T> selected_item;
    int width;
    static constexpr int PADDING = 4;

    std::vector<std::unique_ptr<DropdownItem>> items;

    void clear();

    template<typename U>
    friend class Dropdown;
    friend class PresetDropdown;
};

#include "DropdownMenu.tpp"
#include "DropdownItem.tpp"