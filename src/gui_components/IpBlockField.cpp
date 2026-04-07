#include "IpAddressField.h"
#include "palettes/Palette.h"

IpAddressField::IpBlockField::IpBlockField()
{
    setIndents(2, 0);
    setBorder(juce::BorderSize(0));
    setMultiLine(false);
    setReturnKeyStartsNewLine(false);
    setReadOnly(false);
    setScrollbarsShown(false);
    setCaretVisible(true);
    setPopupMenuEnabled(false);
    setInputRestrictions(3, "0123456789");

    setColour(outlineColourId, juce::Colours::transparentBlack);    // remove textfield outline stroke
    setColour(focusedOutlineColourId, juce::Colours::transparentBlack);
    setColour(textColourId, ColorPalette::Coffee500);
    setColour(highlightedTextColourId, ColorPalette::Coffee500);
    setColour(highlightColourId, ColorPalette::Linen200);
    setColour(juce::CaretComponent::caretColourId, ColorPalette::Coffee500);

    setJustification(juce::Justification::centred);
    setFont(FontPalette::Text_S_Semibold);
    setTextToShowWhenEmpty("000", ColorPalette::Coffee500);
    setSize(32, 24);
}

void IpAddressField::IpBlockField::paint(juce::Graphics& g)
{
    g.setColour(ColorPalette::Linen100);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);
}