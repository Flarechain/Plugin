#pragma once

template <typename T>
Dropdown<T>::Dropdown(int width, T default_item_id, juce::String default_item_text) : button(width, default_item_text),
    menu(width, default_item_id, default_item_text), menu_open(false)
{
    selected_item = Item<T>{default_item_id, default_item_text};
    button.onClick = [this]()
    {
        if (menu_open) { close_menu(); }
        else { open_menu(); }
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
void Dropdown<T>::set_selected_item(T item)
{
    for (const auto& x : menu.items)
    {
        if (x.get()->get_item().id == item)
        {
            selected_item = x.get()->get_item();
            button.setButtonText(selected_item.label);
            menu.set_selected_item(selected_item);
        }
    }
}

template <typename T>
T Dropdown<T>::get_selected_item() const
{
    return selected_item.id;
}

template <typename T>
void Dropdown<T>::open_menu()
{
    menu_open = true;
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
    juce::Desktop::getInstance().addGlobalMouseListener(this);
}

template <typename T>
void Dropdown<T>::close_menu()
{
    menu_open = false;
    menu.setVisible(false);
    juce::Desktop::getInstance().removeGlobalMouseListener(this);
}

template <typename T>
void Dropdown<T>::mouseDown(const juce::MouseEvent& event)
{
    const auto pos = event.getEventRelativeTo(getTopLevelComponent()).getPosition();
    const auto menu_bounds = menu.getBounds();
    const auto button_bounds_in_top = getTopLevelComponent()->getLocalPoint(&button, button.getLocalBounds().getTopLeft());
    const auto button_rect = juce::Rectangle<int>(button_bounds_in_top.getX(), button_bounds_in_top.getY(), button.getWidth(), button.getHeight());

    if (!menu_bounds.contains(pos) && !button_rect.contains(pos))
    {
        close_menu();
    }
}
