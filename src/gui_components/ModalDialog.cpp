#include <juce_gui_basics/juce_gui_basics.h>
#include "ModalDialog.h"

ModalDialog::ModalDialog(juce::String title, juce::String subtitle, const juce::String& primary_action_label, const juce::String& secondary_action_label) :
    title(std::move(title)),
    subtitle(std::move(subtitle)),
    primary_button(primary_action_label, Primary, 112),
    secondary_button(secondary_action_label, Secondary, 112)
{
    icon = IconPalette::Warning(30, ColorPalette::Coffee500);

    primary_button.onClick = [this]()
    {
        if (on_primary_action) { on_primary_action(); }
    };
    secondary_button.onClick = [this]()
    {
        if (on_secondary_action) { on_secondary_action(); }
    };

    constexpr int width = 266;
    const auto title_font = FontPalette::Text_L_Semibold;
    const auto subtitle_font = FontPalette::Text_M_Medium;

    juce::AttributedString title_text;
    title_text.setText(this->title);
    title_text.setColour(ColorPalette::Coffee500);
    title_text.setFont(title_font);
    title_text.setJustification(juce::Justification::topLeft);
    title_text.setLineSpacing(TITLE_LINE_SPACING);
    title_text.setWordWrap(juce::AttributedString::WordWrap::byWord);
    title_layout.createLayout(title_text, width - PADDING * 2);
    title_lines = (title_layout.getNumLines() > MAX_TITLE_LINES) ? MAX_TITLE_LINES : title_layout.getNumLines();
    const int title_height = static_cast<int>(title_font.getHeight() + TITLE_LINE_SPACING) * title_lines;

    juce::AttributedString subtitle_text;
    subtitle_text.setText(this->subtitle);
    subtitle_text.setColour(ColorPalette::Coffee500);
    subtitle_text.setFont(subtitle_font);
    subtitle_text.setJustification(juce::Justification::topLeft);
    subtitle_text.setWordWrap(juce::AttributedString::WordWrap::byWord);
    subtitle_layout.createLayout(subtitle_text, width - PADDING * 2);
    subtitle_lines = (subtitle_layout.getNumLines() > MAX_SUBTITLE_LINES) ? MAX_SUBTITLE_LINES : subtitle_layout.getNumLines();
    const int subtitle_height = static_cast<int>(subtitle_font.getHeight()) * subtitle_lines;

    const int height = icon->getHeight() + GAP_ICON_TEXT + title_height + GAP_TEXT +
        subtitle_height + GAP_TEXT_BUTTONS + primary_button.getHeight() + PADDING * 2;

    addAndMakeVisible(primary_button);
    addAndMakeVisible(secondary_button);
    setSize(width, height);
}

void ModalDialog::paint(juce::Graphics& g)
{
    const auto fill_color = ColorPalette::Linen400;
    const auto title_font = FontPalette::Text_L_Semibold;
    const auto subtitle_font = FontPalette::Text_M_Medium;

    g.setColour(fill_color);
    g.fillRoundedRectangle(0, 0,
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight()),
        16);

    icon->drawAt(g, PADDING, PADDING, 1.0f);

    constexpr int x = PADDING;
    int y = PADDING + icon->getHeight() + GAP_ICON_TEXT;
    for (int i = 0; i < title_lines; i++)
    {
        auto line = title_layout.getLine(i);
        const auto line_height = static_cast<int>(line.getLineBounds().getHeight());
        auto range = line.stringRange;
        juce::String text;
        if (i < title_lines - 1) { text = title.substring(range.getStart(), range.getEnd()); }
        else { text = title.substring(range.getStart()); }

        g.setColour(ColorPalette::Coffee500);
        g.setFont(title_font);
        g.drawFittedText(text,
            x,
            y,
            getWidth() - PADDING * 2,
            line_height,
            juce::Justification::centredLeft,
            1,
            1.0f
        );
        y += line_height + TITLE_LINE_SPACING;
    }

    y += GAP_TEXT;
    g.setFont(subtitle_font);
    g.drawFittedText(subtitle,
        x,
        y,
        getWidth() - PADDING * 2,
        static_cast<int>(subtitle_layout.getHeight()),
        juce::Justification::topLeft,
        MAX_SUBTITLE_LINES,
        1.0f
    );

    const auto content_bounds = juce::Rectangle(PADDING,
        PADDING,
        getWidth() - PADDING * 2,
        getHeight() - PADDING * 2
    );

    juce::RectanglePlacement placement { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    secondary_button.setBounds(placement.appliedTo(secondary_button.getLocalBounds(), content_bounds));

    placement = { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    primary_button.setBounds(placement.appliedTo(primary_button.getLocalBounds(), content_bounds));
}
