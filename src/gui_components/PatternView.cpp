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

    playback_control.on_play = [this]()
    {
        if (on_play) { on_play(); }
    };
    playback_control.on_stop = [this]()
    {
        if (on_stop_playing) { on_stop_playing(); }
    };
    record_button.on_record = [this]()
    {
        if (on_record) { on_record(); }
    };
    record_button.on_stop = [this]()
    {
        if (on_stop_recording) { on_stop_recording(); }
    };

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
        playback_control.setEnabled(true);  // this forces setEnabled to be called on "false" even if it was already on false
        playback_control.setEnabled(false);

        more_options.clear();
        more_options.add_item("Import MIDI file", IconPalette::Import(12, ColorPalette::Coffee500), [this]{ import_midi(); });
    }
    else
    {
        pattern_label.setAlpha(1.0f);
        playback_control.setEnabled(false); // this forces setEnabled to be called on "true" even if it was already on true
        playback_control.setEnabled(true);

        more_options.clear();
        more_options.add_item("Import MIDI file", IconPalette::Import(12, ColorPalette::Coffee500), [this]{ import_midi(); });
        more_options.add_item("Delete pattern", IconPalette::Trash(12, ColorPalette::Coffee500), [this] { delete_pattern(); });
    }
}

void PatternView::set_controls_enabled(const bool enabled)
{
    if (enabled)
    {
        record_button.setEnabled(true);
        more_options.setEnabled(true);
        playback_control.set_play_button_enabled(true);
    }
    else
    {
        record_button.setEnabled(false);
        more_options.setEnabled(false);
        playback_control.set_play_button_enabled(false);
    }
}

void PatternView::import_midi() const
{
    if (on_import)  { on_import(); }
}

void PatternView::delete_pattern() const
{
    if (on_delete)  { on_delete(); }
}

void PatternView::record()
{
    record_button.record();
    playback_control.set_play_button_enabled(false);
    more_options.setEnabled(false);
}

void PatternView::stop_recording()
{
    record_button.stop();
    playback_control.set_play_button_enabled(true);
    more_options.setEnabled(true);
}
