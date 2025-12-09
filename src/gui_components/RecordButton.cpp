#include "RecordButton.h"
#include "palettes/Palette.h"

RecordButton::RecordButton() : Button("RECORD")
{
    icon = IconPalette::Record(6, ColorPalette::Coffee500);

    setSize(86, 24);
}

void RecordButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 8);

    g.setFont(FontPalette::Text_S_Semibold);
    g.setColour(ColorPalette::Coffee500);

    g.drawFittedText(getButtonText(), getLocalBounds(), juce::Justification::centred, 1);

    auto area = getLocalBounds().toFloat();
    icon->setBounds(area.getX(), area.getY(), area.getWidth(), area.getHeight());
    icon->draw(g, 1.0f);
}