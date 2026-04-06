#pragma once

#include "PluginProcessor.h"

#include "../gui_components/Components.h"
#include "../model/Instrument.h"

//==============================================================================
class FlarechainAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit FlarechainAudioProcessorEditor (FlarechainAudioProcessor&);
    ~FlarechainAudioProcessorEditor() override = default;

    //==============================================================================
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
    static constexpr int PADDING = 48;

    void show_pattern_delete_modal(PatternId pattern_id);
    void show_pattern_import_modal(PatternId pattern_id);
    void show_modal();
    void close_modal();
};
