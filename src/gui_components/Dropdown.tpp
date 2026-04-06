#pragma once

template <typename T>
Dropdown<T>::Dropdown(int width, T default_item_id, juce::String default_item_text) : button(width, default_item_text), menu(width, default_item_id, default_item_text)
{
    setWantsKeyboardFocus(true);

    selected_item = Item<T>{default_item_id, default_item_text};
    button.onClick = [this]()
    {
        if (menu.isVisible())
        {
            close_menu();
        }
        else
        {
            open_menu();
        }
    };

    menu.on_change = [this](Item<T> item)
    {
        selected_item = item;
        button.setButtonText(selected_item.label);
        close_menu();
        if (on_change) { on_change(item); }
    };

    addAndMakeVisible(button);
    setSize(button.getWidth(), button.getHeight());
}

template <typename T>
void Dropdown<T>::add_item(T item_id, juce::String item_text, bool selected)
{
    menu.add_item(item_id, item_text, selected);
}

template <typename T>
T Dropdown<T>::get_selected_item() const
{
    return selected_item.id;
}

template <typename T>
void Dropdown<T>::focusLost(FocusChangeType cause)
{
    juce::ignoreUnused(cause);

    // if focus is taken by other components (but not by DropdownButton), close DropdownMenu
    // Note: DropdownButton already closes the menu on click, not on focus gained
    if (!isMouseOver(true)) { close_menu(); }
}

template <typename T>
void Dropdown<T>::open_menu()
{
    auto top_component = getTopLevelComponent();
    const auto button_bounds = button.getLocalBounds();

    top_component->addAndMakeVisible(menu);
    menu.setVisible(true);
    menu.setBounds(
        top_component->getLocalPoint(this, button_bounds.getBottomLeft()).getX(),
        top_component->getLocalPoint(this, button_bounds.getBottomLeft()).getY() + GAP,
        menu.getWidth(),
        menu.getHeight()
    );

    menu.toFront(true);
    grabKeyboardFocus();
}

template <typename T>
void Dropdown<T>::close_menu()
{
    menu.setVisible(false);
    menu.giveAwayKeyboardFocus();
}
