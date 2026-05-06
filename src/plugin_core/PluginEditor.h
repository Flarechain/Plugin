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

    /// Shows a modal dialog for overriding the existing pattern's MIDI with a recorded MIDI sequence.
    void show_pattern_record_modal(PatternId pattern_id);

    /// Shows the already defined modal dialog, on top of everything,
    /// with a darker background and forcing the user to interact with it.
    void show_modal();

    /// Closes the modal dialog.
    void close_modal();

    /// Opens a native OS file chooser in async to select a MIDI file for importing into the given pattern.
    void open_midi_chooser(PatternId pattern_id);

    /// Updates the StatusBar message and synchronizes the UI state accordingly.
    ///
    /// This method evaluates the current application state and:
    /// - Updates the status message shown in the StatusBar
    /// - Enables or disables UI components based on availability
    ///
    /// Examples:
    /// - Disables the "Train Model" button when training is not possible
    /// - Disables the Live Detection page if the AI model is not downloaded
    ///
    /// This function should be called whenever the underlying model or application
    /// state changes.
    void update_status();
};
