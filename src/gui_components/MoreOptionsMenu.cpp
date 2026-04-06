#include "MoreOptionsMenu.h"
#include "palettes/Palette.h"

MoreOptionsMenu::MoreOptionsMenu(int width) : width(width)
{
    setSize(0, 0);
}

void MoreOptionsMenu::paint(juce::Graphics& g)
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

void MoreOptionsMenu::add_item(const juce::String& text, Icon icon, std::function<void()> on_click)
{
    auto item = std::make_unique<MoreOptionsItem>(text, std::move(icon), width - PADDING * 2);
    item->onClick = std::move(on_click);

    items.push_back(std::move(item));
    addAndMakeVisible(*items.back());

    setSize(items.at(0)->getWidth() + PADDING * 2,
        items.at(0)->getHeight() * static_cast<int>(items.size()) + PADDING * 2);

    repaint();
}
