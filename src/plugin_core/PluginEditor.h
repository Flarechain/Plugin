#pragma once

#include "PluginProcessor.h"

#include "../gui_components/Components.h"
#include "../model/Instrument.h"

class FlarechainAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit FlarechainAudioProcessorEditor (FlarechainAudioProcessor&);
    ~FlarechainAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlarechainAudioProcessorEditor)

    FlarechainAudioProcessor& processorRef;

    std::unique_ptr<juce::Drawable> logo;
    juce::Label preset_label;
    PresetDropdown preset_dropdown;
    SaveAsButton save_as_button;
    Dropdown<Instrument> instrument_dropdown;
    Button train_button;
    StatusBar status_bar;
    PageView page_view;
    std::unique_ptr<ModalDialog> modal_dialog;
    juce::DrawableRectangle overlay_background;     // darker background shown when modal_dialog is opened
    std::unique_ptr<juce::FileChooser> midi_chooser;    // file chooser window to select which MIDI file to import
    static constexpr int PADDING = 48;

    /// Shows a modal dialog for deleting a pattern.
    void show_pattern_delete_modal(PatternId pattern_id);

    /// Shows a modal dialog for overriding the existing pattern's MIDI with the content of a MIDI file.
    void show_pattern_import_modal(PatternId pattern_id);

    /// Shows the already defined modal dialog, on top of everything,
    /// with a darker background and forcing the user to interact with it.
    void show_modal();

    /// Closes the modal dialog.
    void close_modal();

    /// Opens a native OS file chooser to select a MIDI file for importing into the given pattern.
    void open_midi_chooser(PatternId pattern_id);

    /// Tries to convert the content of a file into a MIDI sequence.
    ///
    /// Will return `std::nullopt` if the file has no valid MIDI data.
    ///
    /// If the MIDI file has data on multiple tracks, all messages will be merged into a single MIDIMessageSequence.
    static std::optional<juce::MidiMessageSequence> load_midi(const juce::File& file);
};
