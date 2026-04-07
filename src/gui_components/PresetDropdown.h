#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DropdownMenu.h"

typedef juce::uint8 PresetId;

class PresetDropdown : public juce::Component
{
public:
    /// Creates a new PresetDropdown.
    ///
    /// To add presets to the menu, use add_preset().
    PresetDropdown();

    ~PresetDropdown() override = default;

    /// Adds a new preset to the menu, specifying if it should be selected.
    ///
    /// Since only one preset can be the selected one, the last one added with `selected = true` will become
    /// the selected preset.
    void add_preset(PresetId id, const juce::String& preset_name, bool selected);

    PresetId get_selected_preset() const;

    /// Clears the DropdownMenu, leaving it empty.
    void clear();

    std::function<void(Item<PresetId>)> on_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetDropdown)

    /// PresetDropdown's subcomponent representing the clickable button that displays the currently selected preset.
    class PresetDropdownButton : public juce::Button
    {
    public:
        explicit PresetDropdownButton(const juce::String& text);

        ~PresetDropdownButton() override = default;

        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
        void resized() override;

        /// Changes text of PresetDropdownButton, resizing the component dynamically.
        void setDropdownText(const juce::String& text);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetDropdownButton)

        // setButtonText() is only allowed inside this class, not on PresetDropdownButton API
        // To change buttonText, use setDropdownText() instead
        using juce::Button::setButtonText;

        Icon icon;
        static constexpr int PADDING_X = 8;
        static constexpr int PADDING_Y = 2;
        static constexpr int GAP = 8;
        static constexpr int MAX_WIDTH = 154;
    };

    Item<PresetId> selected_preset;

    PresetDropdownButton button;
    DropdownMenu<PresetId> menu;
    static constexpr int GAP = 4;

    void focusLost(FocusChangeType cause) override;
    void open_menu();
    void close_menu();
};