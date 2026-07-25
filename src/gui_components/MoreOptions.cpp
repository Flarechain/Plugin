#include "MoreOptions.h"

MoreOptions::MoreOptions(const int menu_width) : menu(menu_width), menu_open(false)
{
    button.onClick = [this]()
    {
        if (menu_open) { close_menu(); }
        else { open_menu(); }
    };

    addAndMakeVisible(button);
    setSize(button.getWidth(), button.getHeight());
}

void MoreOptions::add_item(const juce::String& item_text, Icon item_icon, const std::function<void()>& on_click)
{
    menu.add_item(item_text, std::move(item_icon), [this, on_click]()
    {
        close_menu();
        juce::MessageManager::callAsync(on_click);
    });
}

void MoreOptions::clear()
{
    menu.clear();
}

void MoreOptions::open_menu()
{
    menu_open = true;
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
    juce::Desktop::getInstance().addGlobalMouseListener(this);
}

void MoreOptions::close_menu()
{
    menu_open = false;
    menu.setVisible(false);
    juce::Desktop::getInstance().removeGlobalMouseListener(this);
}

void MoreOptions::mouseDown(const juce::MouseEvent& event)
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