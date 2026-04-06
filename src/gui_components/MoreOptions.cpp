#include "MoreOptions.h"

MoreOptions::MoreOptions(const int menu_width) : menu(menu_width)
{
    setWantsKeyboardFocus(true);

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

    addAndMakeVisible(button);
    setSize(button.getWidth(), button.getHeight());
}

void MoreOptions::add_item(const juce::String& item_text, Icon item_icon, const std::function<void()>& on_click)
{
    menu.add_item(item_text, std::move(item_icon), on_click);
}

void MoreOptions::focusLost(FocusChangeType cause)
{
    juce::ignoreUnused(cause);

    // if focus is taken by other components (but not by MoreOptionsButton), close MoreOptionsMenu
    // Note: MoreOptionsButton already closes the menu on click, not on focus gained
    if (!isMouseOver(true)) { close_menu(); }
}

void MoreOptions::open_menu()
{
    const auto top_component = getTopLevelComponent();
    const auto button_bounds = button.getLocalBounds();

    top_component->addAndMakeVisible(menu);
    menu.setVisible(true);
    menu.setBounds(
        top_component->getLocalPoint(this, button_bounds.getBottomRight()).getX() - menu.getWidth(),
        top_component->getLocalPoint(this, button_bounds.getBottomRight()).getY() + GAP,
        menu.getWidth(),
        menu.getHeight()
    );

    menu.toFront(true);
    grabKeyboardFocus();
}

void MoreOptions::close_menu()
{
    menu.setVisible(false);
    menu.giveAwayKeyboardFocus();
}