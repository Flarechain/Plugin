#pragma once

#include "PresetDropdown.h"

PresetId INIT_ID = 0;

PresetDropdown::PresetDropdown() : button("Init"), menu(154, 0, "Init")
{
    setWantsKeyboardFocus(true);
    setInterceptsMouseClicks(false, true);

    selected_preset = Item<PresetId>{INIT_ID, "Init"};
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

    menu.on_change = [this](Item<PresetId> preset)
    {
        selected_preset = std::move(preset);
        button.setDropdownText(selected_preset.label);
        setSize(button.getWidth(), button.getHeight());
        close_menu();
        if (on_change) { on_change(selected_preset); }
    };

    addAndMakeVisible(button);
    setSize(button.getWidth(), button.getHeight());
}

void PresetDropdown::add_preset(PresetId item_id, juce::String item_text, bool selected)
{
    if (item_id != INIT_ID) { menu.add_item(item_id, item_text, selected); }
}

PresetId PresetDropdown::get_selected_preset() const
{
    return selected_preset.id;
}

void PresetDropdown::clear()
{
    menu.clear();
    selected_preset = Item<PresetId>{INIT_ID, "Init"};
    menu.add_item(INIT_ID, "Init", true);
    button.setDropdownText("Init");
}

void PresetDropdown::focusLost(FocusChangeType cause)
{
    // if focus is taken by other components (but not by DropdownButton) close DropdownMenu
    // Note: DropdownButton already closes menu on click, not on focus gained
    if (!isMouseOver(true)) { close_menu(); }
}

void PresetDropdown::open_menu()
{
    auto top_component = getTopLevelComponent();
    auto button_bounds = button.getLocalBounds();

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

void PresetDropdown::close_menu()
{
    menu.setVisible(false);
    menu.giveAwayKeyboardFocus();
}
