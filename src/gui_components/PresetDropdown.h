#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DropdownMenu.h"
#include "PresetDropdownButton.h"

typedef juce::uint8 PresetId;

class PresetDropdown : public juce::Component
{
public:
    PresetDropdown();
    ~PresetDropdown() override = default;

    void add_preset(PresetId id, juce::String preset_name, bool selected);
    PresetId get_selected_preset() const;
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