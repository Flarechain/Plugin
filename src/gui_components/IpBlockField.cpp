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

    setJustification(juce::Justification::centred);
    setFont(FontPalette::Text_S_Semibold);
    setTextToShowWhenEmpty("000", ColorPalette::Coffee300);
    setSize(32, 24);
}

void IpAddressField::IpBlockField::paint(juce::Graphics& g)
{
    g.setColour(ColorPalette::Linen100);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);
}

void IpAddressField::IpBlockField::focusLost(FocusChangeType cause)
{
    juce::ignoreUnused(cause);

    std::optional<juce::uint8> new_value = std::nullopt;

    if (getText() == "")
    {
        clear();
    }
    else
    {
        int num = getText().getIntValue();
        num = std::clamp(num, 0, 255);
        setText(juce::String(num), juce::dontSendNotification);

        new_value = static_cast<juce::uint8>(num);
    }

    if (on_change && new_value != current_value)
    {
        current_value = new_value;
        on_change(current_value);
    }
}

bool IpAddressField::IpBlockField::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::upKey || key == juce::KeyPress::downKey ||
        key == juce::KeyPress::leftKey || key == juce::KeyPress::rightKey)
    {
        if (on_arrow_key) { on_arrow_key(key); }
        return true;
    }

    return juce::TextEditor::keyPressed(key);
}
