#pragma once

#include "palettes/Palette.h"

template <typename T>
DropdownMenu<T>::DropdownMenu(const int width, T default_item_id, const juce::String& default_item_text) : width(width)
{
    add_item(default_item_id, default_item_text, true);
}

template <typename T>
void DropdownMenu<T>::paint(juce::Graphics& g)
{
    const auto fill_color = ColorPalette::Linen400;
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        8);

    constexpr int x = PADDING;
    int y = PADDING;
    for (const auto& item : items)
    {
        item->setTopLeftPosition(x, y);
        y += item->getHeight();
    }
}

template <typename T>
void DropdownMenu<T>::add_item(T id, juce::String text, const bool selected)
{
    auto item = Item<T>{id, text};
    auto dropdown_item = std::make_unique<DropdownItem>(item, width - PADDING * 2);
    dropdown_item->setRadioGroupId(1);
    dropdown_item->onClick = [this, id, text]()
    {
        const auto item = Item<T>{id, text};
        selected_item = item;
        for (const auto& x : items)
        {
            x->setToggleState(x->get_item().id == item.id, juce::dontSendNotification);
        }
        if (on_change) { on_change(item); }
    };

    items.push_back(std::move(dropdown_item));
    addAndMakeVisible(*items.back());

    if (items.size() == 1 || selected)
    {
        set_selected_item(Item<T>{id, text});
    }

    setSize(items.at(0)->getWidth() + PADDING * 2,
        items.at(0)->getHeight() * items.size() + PADDING * 2);

    repaint();
}

template <typename T>
void DropdownMenu<T>::set_selected_item(Item<T> item)
{
    selected_item = item;
    for (const auto& x : items)
    {
        x->setToggleState(x->get_item().id == item.id, juce::dontSendNotification);
        x->repaint();
    }
}

template <typename T>
T DropdownMenu<T>::get_selected_item() const
{
    return selected_item.id;
}

template <typename T>
void DropdownMenu<T>::clear()
{
    items.clear();
    removeAllChildren();
    setSize(0, 0);
}

// TODO: make dropdownmenu scrollable with max height
