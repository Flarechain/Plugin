#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DropdownMenu.h"
#include "PresetDropdownButton.h"

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

    Item<PresetId> selected_preset;

    PresetDropdownButton button;
    DropdownMenu<PresetId> menu;
    static constexpr int GAP = 4;

    void focusLost(FocusChangeType cause) override;
    void open_menu();
    void close_menu();
};