#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "TabBar.h"
#include "PatternListView.h"
#include "LogPanel.h"

class PageView : public juce::Component
{
public:
    /// Creates a new PageView with a TabBar to navigate between pages.
    explicit PageView(const PatternList& pattern_list);

    ~PageView() override = default;

    void resized() override;

    /// Updates the component state to reflect the current model and triggers a repaint.
    void refresh() const;

    std::function<void(PatternId id)> on_pattern_import;
    std::function<void(PatternId id)> on_pattern_delete;

    /// Called when a pattern's IP address input changes.
    ///
    /// @param ip An optional IP address:
    /// - `std::nullopt` if the input provided by the user is incomplete
    /// - a valid `juce::IPAddress` if all fields are filled
    std::function<void(PatternId id, std::optional<juce::IPAddress> ip)> on_ip_change;

    /// Called when a pattern's OSC message input changes.
    ///
    /// @param osc An optional OSC message:
    /// - `std::nullopt` if the input provided by the user is not valid or incomplete
    /// - a `juce::OSCMessage` if the input is a valid OSC message
    std::function<void(PatternId id, std::optional<juce::OSCMessage> osc)> on_osc_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PageView)

    Page current_page;

    TabBar tab_bar;
    PatternListView pattern_list_view;
    LogPanel log_panel;
};