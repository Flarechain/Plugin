#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "BinaryData.h"
#include "../gui_components/palettes/Palette.h"

FlarechainAudioProcessorEditor::FlarechainAudioProcessorEditor (FlarechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p),
    instrument_dropdown(90, Instrument::Guitar, "Guitar"),
    train_button("Train model", Primary),
    status_bar(Warning, "No pattern has been set up"),
    page_view(p.get_pattern_list())
{
    juce::ignoreUnused(processorRef);
    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);

    logo = IconPalette::Logo(20, ColorPalette::Coffee500);

    const auto font = FontPalette::Text_S_Medium;
    preset_label.setText("PRESET", juce::dontSendNotification);
    preset_label.setFont(font);
    preset_label.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    preset_label.setSize(70, static_cast<int>(font.getHeight()));
    preset_label.setMinimumHorizontalScale(1.0f); // disable font stretching
    preset_label.setInterceptsMouseClicks(false, true);

    instrument_dropdown.add_item(Instrument::Keyboard, "Keyboard", false);
    instrument_dropdown.add_item(Instrument::Drums, "Drums", false);

    preset_dropdown.on_change = [this](Item<PresetId> preset)
    {
        resized();
    };

    page_view.get_pattern_list_view().on_import = [this](const PatternId id)
    {
        if (processorRef.get_pattern(id).has_empty_midi())
        {
            open_midi_chooser(id);
        }
        else
        {
            show_pattern_import_modal(id);
        }
    };
    page_view.get_pattern_list_view().on_delete = [this](const PatternId id)
    {
        show_pattern_delete_modal(id);
    };
    page_view.get_pattern_list_view().on_play = [this](const PatternId id)
    {
        processorRef.play_pattern(id);
    };
    page_view.get_pattern_list_view().on_stop_playing = [this](const PatternId id)
    {
        processorRef.stop_playing();
    };
    page_view.get_pattern_list_view().on_ip_change = [this](const PatternId pattern_id, std::optional<juce::IPAddress> ip)
    {
        processorRef.set_ip_address(pattern_id, ip);
        update_status();
    };
    page_view.get_pattern_list_view().on_osc_change = [this](const PatternId pattern_id, std::optional<juce::OSCMessage> osc)
    {
        processorRef.set_osc_message(pattern_id, std::move(osc));
        update_status();
    };

    processorRef.on_playback_start = [this](PatternId pattern_id)
    {
        page_view.get_pattern_list_view().play(pattern_id);
    };
    processorRef.on_playback_stop = [this](PatternId pattern_id)
    {
        page_view.get_pattern_list_view().stop_playing(pattern_id);
    };

    update_status();

    overlay_background.setFill(juce::FillType(juce::Colours::black.withAlpha(0.25f)));
    addChildComponent(overlay_background);

    addAndMakeVisible(preset_label);
    addAndMakeVisible(preset_dropdown);
    addAndMakeVisible(save_as_button);
    addAndMakeVisible(instrument_dropdown);
    addAndMakeVisible(train_button);
    addAndMakeVisible(status_bar);
    addAndMakeVisible(page_view);
    setSize(800, 600);
}

void FlarechainAudioProcessorEditor::paint (juce::Graphics& g)
{
    const juce::Image backgroundImage = juce::ImageFileFormat::loadFrom(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImage(backgroundImage, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);

    logo->drawAt(g,
        static_cast<float>(getWidth() / 2.0) - static_cast<float>(logo->getWidth() / 2.0),
        PADDING,
        1.0f
    );
}

void FlarechainAudioProcessorEditor::resized()
{
    // header section
    const auto header_bounds = juce::Rectangle<int>(PADDING, PADDING, getWidth() - PADDING * 2, 76);

    int gap = 4;
    auto preset_bounds = juce::Rectangle<int>(0,
        0,
        190,
        preset_label.getHeight() + gap + preset_dropdown.getHeight());
    juce::RectanglePlacement placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    preset_bounds = placement.appliedTo(preset_bounds, header_bounds);

    placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yTop | juce::RectanglePlacement::doNotResize };
    preset_label.setBounds(placement.appliedTo(preset_label.getLocalBounds(), preset_bounds));

    placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    preset_dropdown.setBounds(placement.appliedTo(preset_dropdown.getLocalBounds(), preset_bounds));

    gap = 16;
    const auto save_as_bounds = preset_bounds.withTrimmedLeft(preset_dropdown.getWidth() + gap)
        .withTrimmedTop(preset_bounds.getHeight() - preset_dropdown.getHeight());
    placement = { juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    save_as_button.setBounds(placement.appliedTo(save_as_button.getLocalBounds(), save_as_bounds));

    gap = 8;
    auto training_bounds = juce::Rectangle<int>(0,
        0,
        std::max(std::max(instrument_dropdown.getWidth(), train_button.getWidth()), status_bar.getWidth()),
        instrument_dropdown.getHeight() + train_button.getHeight() + status_bar.getHeight() + gap * 2);

    placement = { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    training_bounds = placement.appliedTo(training_bounds, header_bounds);

    juce::FlexBox training_flex;
    training_flex.flexDirection = juce::FlexBox::Direction::column;
    training_flex.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    training_flex.alignItems = juce::FlexBox::AlignItems::flexEnd;

    training_flex.items.add(juce::FlexItem(
        static_cast<float>(instrument_dropdown.getWidth()),
        static_cast<float>(instrument_dropdown.getHeight()),
        instrument_dropdown)
    );

    training_flex.items.add(juce::FlexItem(
        static_cast<float>(train_button.getWidth()),
        static_cast<float>(train_button.getHeight()),
        train_button)
    );

    training_flex.items.add(juce::FlexItem(
        static_cast<float>(status_bar.getWidth()),
        static_cast<float>(status_bar.getHeight()),
        status_bar)
    );
    training_flex.performLayout(training_bounds.toFloat());

    // content section
    auto content_bounds = juce::Rectangle<int>(PADDING,
        getHeight() - PADDING - page_view.getHeight(),
        getWidth() - PADDING * 2,
        page_view.getHeight()
    );

    placement = { juce::RectanglePlacement::xMid | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::doNotResize };
    content_bounds = placement.appliedTo(page_view.getLocalBounds(), content_bounds);
    page_view.setBounds(content_bounds);

    overlay_background.setBounds(getLocalBounds());
    overlay_background.setRectangle(juce::Parallelogram(getLocalBounds().toFloat()));
}

void FlarechainAudioProcessorEditor::show_pattern_import_modal(PatternId pattern_id)
{
    const auto pattern_name = processorRef.get_pattern(pattern_id).get_name();
    modal_dialog = std::make_unique<ModalDialog>("Are you sure you want to import MIDI into Pattern " + pattern_name + "?",
        "This will overwrite the existing pattern and cannot be undone.",
        "Import MIDI",
        "Cancel"
    );

    modal_dialog->on_primary_action = [this, pattern_id]()
    {
        close_modal();
        open_midi_chooser(pattern_id);
    };
    modal_dialog->on_secondary_action = [this]()
    {
        close_modal();
    };

    show_modal();
}

void FlarechainAudioProcessorEditor::show_pattern_delete_modal(PatternId pattern_id)
{
    const auto pattern_name = processorRef.get_pattern(pattern_id).get_name();
    modal_dialog = std::make_unique<ModalDialog>("Are you sure you want to delete Pattern " + pattern_name + "?",
        "This cannot be undone.",
        "Delete pattern",
        "Cancel"
    );

    modal_dialog->on_primary_action = [this, pattern_id]()
    {
        close_modal();
        processorRef.delete_pattern(pattern_id);
        page_view.refresh();
        update_status();
    };
    modal_dialog->on_secondary_action = [this]()
    {
        close_modal();
    };

    show_modal();
}

void FlarechainAudioProcessorEditor::show_modal()
{
    if (modal_dialog == nullptr) return;

    overlay_background.setVisible(true);
    overlay_background.setInterceptsMouseClicks(true, true);
    overlay_background.toFront(false);

    addAndMakeVisible(*modal_dialog);
    modal_dialog->toFront(true);
    modal_dialog->enterModalState(true);

    const juce::RectanglePlacement placement { juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize };
    modal_dialog->setBounds(placement.appliedTo(modal_dialog->getLocalBounds(), getLocalBounds()));
}

void FlarechainAudioProcessorEditor::close_modal()
{
    if (modal_dialog != nullptr)
    {
        overlay_background.setVisible(false);
        modal_dialog->exitModalState(0);
        removeChildComponent(modal_dialog.get());
        modal_dialog.reset();
    }

    repaint();
}

void FlarechainAudioProcessorEditor::open_midi_chooser(const PatternId pattern_id)
{
    if (midi_chooser != nullptr) return;

    const auto pattern_name = processorRef.get_pattern(pattern_id).get_name();
    midi_chooser = std::make_unique<juce::FileChooser>(
        "Select a MIDI file to import into Pattern " + pattern_name + ".",
        juce::File{},
        "*.mid;*.midi"
    );

    midi_chooser->launchAsync(
        juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles,
        [this, pattern_id](const juce::FileChooser& file_chooser)
        {
            const auto file = file_chooser.getResult();
            if (file.existsAsFile())
            {
                auto midi = FlarechainAudioProcessor::load_midi(file);
                if (midi.has_value())
                {
                    processorRef.set_midi(pattern_id, midi.value());
                    page_view.refresh();
                    update_status();
                }
            }

            midi_chooser.reset();
        }
    );
}

void FlarechainAudioProcessorEditor::update_status()
{
    if (processorRef.get_active_patterns().empty())
    {
        status_bar.set_status(Warning, "No pattern has been set up");
        train_button.setEnabled(false);
        return;
    }
    if (processorRef.is_model_downloaded())
    {
        status_bar.set_status(Ok, "Model ready");
        train_button.setEnabled(false);
        return;
    }

    train_button.setEnabled(true);

    const auto patterns_with_invalid_ip = processorRef.get_patterns_with_invalid_ip();
    const auto patterns_with_invalid_osc = processorRef.get_patterns_with_invalid_osc();

    if (patterns_with_invalid_ip.empty() && patterns_with_invalid_osc.empty())
    {
        status_bar.set_status(Warning, "Model not trained");
        return;
    }

    juce::String status_message;
    if (!patterns_with_invalid_ip.empty() && patterns_with_invalid_osc.empty())
    {
        status_message = "Invalid IP for ";
    }
    else if (patterns_with_invalid_ip.empty() && !patterns_with_invalid_osc.empty())
    {
        status_message = "Invalid OSC for ";
    }
    else if (!patterns_with_invalid_ip.empty() && !patterns_with_invalid_osc.empty())
    {
        status_message = "Invalid IP/OSC for ";
    }

    std::set<PatternId> patterns_with_invalid_ip_or_osc;
    patterns_with_invalid_ip_or_osc.insert(patterns_with_invalid_ip.begin(), patterns_with_invalid_ip.end());
    patterns_with_invalid_ip_or_osc.insert(patterns_with_invalid_osc.begin(), patterns_with_invalid_osc.end());

    juce::uint32 i = 1;
    for (const auto pattern_id : patterns_with_invalid_ip_or_osc)
    {
        if (i > 1) status_message << ", ";
        status_message << processorRef.get_pattern(pattern_id).get_name();
        i++;
    }

    status_bar.set_status(Warning, status_message);
}