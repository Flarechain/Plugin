#include "TextField.h"
#include "palettes/Palette.h"

TextField::TextField()
{
    setIndents(10, 0);
    setMultiLine(false);
    setReturnKeyStartsNewLine(false);
    setReadOnly(false);
    setScrollbarsShown(false);
    setCaretVisible(true);
    setPopupMenuEnabled(false);

    onReturnKey = [this]()
    {
        giveAwayKeyboardFocus();
    };

    setColour(outlineColourId, juce::Colours::transparentBlack);    // remove textfield outline stroke
    setColour(focusedOutlineColourId, juce::Colours::transparentBlack);
    setColour(textColourId, ColorPalette::Coffee500);
    setColour(highlightedTextColourId, ColorPalette::Coffee500);
    setColour(highlightColourId, ColorPalette::Linen200);
    setColour(juce::CaretComponent::caretColourId, ColorPalette::Coffee500);

    setJustification(juce::Justification::centredLeft);
    setFont(FontPalette::Text_S_Medium);
    setTextToShowWhenEmpty("OSC message", ColorPalette::Coffee300);
    setSize(288, 24);
}

void TextField::paint(juce::Graphics& g)
{
    g.setColour(ColorPalette::Linen100);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);
}

void TextField::focusLost(FocusChangeType cause)
{
    juce::ignoreUnused(cause);

    juce::String new_text;

    if (getText() == "")
    {
        clear();
    }
    else
    {
        new_text = getText();
    }

    if (on_change && new_text != current_text)
    {
        current_text = new_text;
        on_change(current_text);
    }
}