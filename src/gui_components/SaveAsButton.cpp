#include "SaveAsButton.h"
#include "palettes/Palette.h"

SaveAsButton::SaveAsButton() : Button("Save as")
{
    icon = IconPalette::Save(12, ColorPalette::Coffee500);
    setSize(icon->getWidth() + PADDING * 2, icon->getHeight() + PADDING * 2);
}

void SaveAsButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100.withAlpha(0.0f);
    if (isButtonDown) { fill_color = ColorPalette::Linen200; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen100; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 4);

    g.setColour(ColorPalette::Coffee500);
    icon->drawAt(g,
        static_cast<float>(getWidth() - icon->getWidth()) / 2,
        static_cast<float>(getHeight() - icon->getHeight()) / 2,
        1.0f
    );
}