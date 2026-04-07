#include "MoreOptionsMenu.h"
#include "palettes/Palette.h"

MoreOptionsMenu::MoreOptionsItem::MoreOptionsItem(const juce::String& text, Icon icon, int width) : Button(text), icon(std::move(icon))
{
    const auto font = FontPalette::Text_S_Medium;
    setSize(width, static_cast<int>(font.getHeight()) + PADDING * 2);
}

void MoreOptionsMenu::MoreOptionsItem::paintButton(juce::Graphics& g, const bool isMouseOverButton, const bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100.withAlpha(0.0f);
    if (isButtonDown) { fill_color = ColorPalette::Linen700; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen600; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        4);

    const auto content_bounds = getLocalBounds().reduced(PADDING, PADDING);

    g.setFont(FontPalette::Text_S_Medium);
    g.setColour(ColorPalette::Coffee500);
    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1.0f
    );
    g.drawText(getButtonText(),
        content_bounds.withTrimmedLeft(icon->getWidth() + GAP),
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid,
        true
    );
}