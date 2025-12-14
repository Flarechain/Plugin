#include "RecordButton.h"
#include "palettes/Palette.h"

RecordButton::RecordButton() : Button("RECORD")
{
    icon = IconPalette::Record(6, ColorPalette::Coffee500);
    is_recording = false;
    setSize(86, 24);
}

void RecordButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    // button fill color
    auto fill_color = ColorPalette::Linen100;
    if (isButtonDown) { fill_color = ColorPalette::Linen300; }
    else if (isMouseOverButton) { fill_color = ColorPalette::Linen200; }

    // button rectangle
    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8);

    const auto font = FontPalette::Text_S_Semibold;

    // (icon + text) position, padding and gap
    constexpr int gap = 8;
    const int icon_width = icon->getWidth();
    const int text_width = static_cast<int>(juce::GlyphArrangement::getStringWidth(font, getButtonText()));
    const int text_height = static_cast<int>(font.getHeight());
    auto content_bounds = juce::Rectangle(0,
        0,
        icon_width + gap + text_width,
        text_height
    );
    juce::RectanglePlacement placement { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
    content_bounds = placement.appliedTo(content_bounds, getLocalBounds());

    g.setFont(font);
    g.setColour(ColorPalette::Coffee500);
    g.drawFittedText(getButtonText(), content_bounds, juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid, 1.0f);
    icon->drawWithin(g,
        content_bounds.toFloat(),
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}

void RecordButton::record()
{
    is_recording = true;
    icon = IconPalette::Stop(6, ColorPalette::Coffee500);
    setButtonText("STOP");
}

void RecordButton::stop()
{
    is_recording = false;
    icon = IconPalette::Record(6, ColorPalette::Coffee500);
    setButtonText("RECORD");
}