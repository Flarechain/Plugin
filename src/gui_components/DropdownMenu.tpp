#pragma once

#include "palettes/Palette.h"

template <typename T>
DropdownMenu<T>::DropdownMenu(const int width, T default_item_id, juce::String default_item_text) : width(width)
{
    add_item(default_item_id, default_item_text);

    items.at(0)->setToggleState(true, juce::dontSendNotification);
    selected_item = Item<T>{default_item_id, default_item_text};
}

template <typename T>
void DropdownMenu<T>::paint(juce::Graphics& g)
{
    auto fill_color = ColorPalette::Linen400;
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        8);

    constexpr int x = PADDING;
    int y = PADDING;
    for (auto& item : items)
    {
        item->setTopLeftPosition(x, y);
        y += item->getHeight();
    }
}

template <typename T>
void DropdownMenu<T>::add_item(T id, juce::String text)
{
    auto item = std::make_unique<DropdownItem>(text, width - PADDING * 2);
    item->setRadioGroupId(1);
    item->onClick = [this, id, text]()
    {
        set_selected_item(Item<T>{id, text});
    };

    items.push_back(std::move(item));
    addAndMakeVisible(*items.back());

    setSize(items.at(0)->getWidth() + PADDING * 2,
        items.at(0)->getHeight() * items.size() + PADDING * 2);

    repaint();
}

template <typename T>
void DropdownMenu<T>::set_selected_item(Item<T> item)
{
    if (selected_item == item) return;

    selected_item = item;
    if (on_change) { on_change(item); }
}

template <typename T>
T DropdownMenu<T>::get_selected_item() const
{
    return selected_item.id;
}