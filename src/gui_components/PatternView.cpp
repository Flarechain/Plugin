#include "PatternView.h"
#include "palettes/Palette.h"

PatternView::PatternView(const Pattern& pattern) : pattern(pattern), more_options(126)
{
    setInterceptsMouseClicks(false, true);

    const auto font = FontPalette::Text_L_Semibold;
    const auto text = juce::String("Pattern ") + pattern.get_name();
    pattern_label.setText(text, juce::dontSendNotification);
    pattern_label.setFont(font);
    pattern_label.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    pattern_label.setSize(100, static_cast<int>(font.getHeight()));
    pattern_label.setMinimumHorizontalScale(1.0f); // disable font stretching
    pattern_label.setInterceptsMouseClicks(false, true);

    refresh();

    addAndMakeVisible(pattern_label);
    addAndMakeVisible(record_button);
    addAndMakeVisible(more_options);
    addAndMakeVisible(playback_control);

    constexpr int gap = 8;
    const int top_row_height = std::max(pattern_label.getHeight(),
                                        std::max(record_button.getHeight(), more_options.getHeight()));
    setSize(playback_control.getWidth(), top_row_height + gap + playback_control.getHeight());
}

void PatternView::resized()
{
    // record_button + more_options_button
    constexpr int buttons_top_gap = 8;
    auto buttons_top_bounds = juce::Rectangle(
            record_button.getWidth() + buttons_top_gap + more_options.getWidth(),
            std::max(record_button.getHeight(), more_options.getHeight())
    );

    // pattern_label + (record_button + more_options_button)
    auto top_row_bounds = juce::Rectangle(
            playback_control.getWidth(),
            std::max(pattern_label.getHeight(), std::max(record_button.getHeight(), more_options.getHeight()))
    );

    const juce::RectanglePlacement buttons_top_placement { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    buttons_top_bounds = buttons_top_placement.appliedTo(buttons_top_bounds, top_row_bounds);

    const juce::RectanglePlacement top_row_placement { juce::RectanglePlacement::yTop | juce::RectanglePlacement::xMid | juce::RectanglePlacement::doNotResize };
    top_row_bounds = top_row_placement.appliedTo(top_row_bounds, getLocalBounds());

    const juce::RectanglePlacement bottom_placement { juce::RectanglePlacement::yBottom | juce::RectanglePlacement::xMid | juce::RectanglePlacement::doNotResize };
    playback_control.setBounds(bottom_placement.appliedTo(playback_control.getLocalBounds(), getLocalBounds()));

    const juce::RectanglePlacement pattern_label_placement { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    pattern_label.setBounds(pattern_label_placement.appliedTo(pattern_label.getLocalBounds(), top_row_bounds));

    const juce::RectanglePlacement record_button_placement { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    record_button.setBounds(record_button_placement.appliedTo(record_button.getLocalBounds(), buttons_top_bounds));

    const juce::RectanglePlacement more_options_button_placement { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    more_options.setBounds(more_options_button_placement.appliedTo(more_options.getLocalBounds(), buttons_top_bounds));
}

void PatternView::refresh()
{
    if (pattern.has_empty_midi())
    {
        pattern_label.setAlpha(0.2f);
        playback_control.setAlpha(0.2f);
        playback_control.setEnabled(false);

        more_options.clear();
        more_options.add_item("Import MIDI file", IconPalette::Import(12, ColorPalette::Coffee500), [this]{ import_midi(); });
    }
    else
    {
        pattern_label.setAlpha(1.0f);
        playback_control.setAlpha(1.0f);
        playback_control.setEnabled(true);

        more_options.clear();
        more_options.add_item("Import MIDI file", IconPalette::Import(12, ColorPalette::Coffee500), [this]{ import_midi(); });
        more_options.add_item("Delete pattern", IconPalette::Trash(12, ColorPalette::Coffee500), [this] { delete_pattern(); });
    }
    repaint();
}

void PatternView::import_midi() const
{
    if (on_import)  { on_import(); }
}

void PatternView::delete_pattern() const
{
    if (on_delete)  { on_delete(); }
}
