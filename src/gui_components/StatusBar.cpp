#include "StatusBar.h"
#include "palettes/Palette.h"

StatusBar::StatusBar(Status status, juce::String  message) : status(status), message(std::move(message))
{
    setInterceptsMouseClicks(false, true);

    if (status == Ok)
    {
        icon = IconPalette::Check(12, ColorPalette::Coffee500);
    }
    else if (status == Warning)
    {
        icon = IconPalette::Attention(14, ColorPalette::Coffee500);
    }

    setSize(190, 16);
}

void StatusBar::paint(juce::Graphics& g)
{
    const auto font = FontPalette::Text_S_Medium;
    constexpr int gap = 8;

    const int icon_width = (icon != nullptr) ? icon->getWidth() : 0;

    g.setColour(ColorPalette::Coffee500);
    g.setFont(font);
    g.drawFittedText(message,
        getLocalBounds().withTrimmedLeft(icon_width + gap),
        juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1,
        1.0f
    );

    if (icon != nullptr)
    {
        const float icon_x = (getWidth() - font.getStringWidth(message) - gap - icon->getWidth()) > 0 ?
        static_cast<float>(getWidth() - font.getStringWidth(message) - gap - icon->getWidth()) : 0;
        const float icon_y = static_cast<float>(getHeight() - icon->getHeight()) / 2;
        icon->drawAt(g,
            icon_x,
            icon_y,
            1.0f);
    }
}

void StatusBar::set_status(Status status, juce::String message)
{
    this->status = status;
    this->message = std::move(message);

    if (status == Ok)
    {
        icon = IconPalette::Check(12, ColorPalette::Coffee500);
    }
    else if (status == Warning)
    {
        icon = IconPalette::Attention(14, ColorPalette::Coffee500);
    }
    else
    {
        icon = nullptr;
    }

    repaint();
}
