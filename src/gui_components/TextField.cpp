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
    setText(getText(), juce::dontSendNotification);
}

void TextField::setText(const juce::String& newText, bool sendTextChangeMessage)
{
    TextEditor::setText(newText, sendTextChangeMessage);

    if (newText == "")
    {
        clear();
    }

    if (on_change && newText != current_text)
    {
        current_text = newText;
        on_change(current_text);
    }
}
