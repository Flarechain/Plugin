#include "TabBar.h"

#include "palettes/ColorPalette.h"

constexpr int STROKE_WIDTH = 2;

TabBar::TabBar(std::vector<Page> pages)
{
    int width = STROKE_WIDTH * 2;
    for (auto& page : pages)
    {
        auto tab = std::make_unique<ToggleButton>(toString(page));
        tab->setRadioGroupId(1);
        width += tab->getWidth();

        tab->onClick = [this, page]()
        {
            set_current_page(page);
        };

        tabs.push_back(std::move(tab));
        addAndMakeVisible(*tabs.back());
    }

    tabs.at(0)->setToggleState(true, juce::dontSendNotification);
    current_page = pages.at(0);

    int height = tabs.at(0)->getHeight() + STROKE_WIDTH * 2;
    setSize(width, height);
}

void TabBar::paint(juce::Graphics& g)
{
    g.setColour(ColorPalette::Coffee500);
    g.drawRoundedRectangle(STROKE_WIDTH / 2, STROKE_WIDTH / 2,
        static_cast<float>(getWidth()) - STROKE_WIDTH,
        static_cast<float>(getHeight()) - STROKE_WIDTH,
        (float)(getHeight() / 2),
        STROKE_WIDTH
    );

    int x = STROKE_WIDTH;
    constexpr int y = STROKE_WIDTH;
    for (auto& tab : tabs)
    {
        tab->setTopLeftPosition(x, y);
        x += tab->getWidth();
    }
}

Page TabBar::get_current_page() const
{
    return current_page;
}

void TabBar::set_current_page(Page page)
{
    if (page == current_page) return;

    current_page = page;
    if (on_selection_change) { on_selection_change(page); }
}
