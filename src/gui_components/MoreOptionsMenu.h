#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/Palette.h"

class MoreOptionsMenu : public juce::Component
{
public:
    /// Creates a new MoreOptionsMenu.
    explicit MoreOptionsMenu(int width);

    ~MoreOptionsMenu() override = default;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsMenu)

    /// MoreOptionsMenu's subcomponent for displaying a single item.
    class MoreOptionsItem : public juce::Button
    {
    public:
        MoreOptionsItem(const juce::String& text, Icon icon, int width);

        ~MoreOptionsItem() override = default;

        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoreOptionsItem)

        Icon icon;
        static constexpr int PADDING = 4;
        static constexpr int GAP = 8;
    };

    int width;
    static constexpr int PADDING = 4;

    std::vector<std::unique_ptr<MoreOptionsItem>> items;

    /// Adds a new item to the menu with the given text, icon, and action to do when chosen.
    void add_item(const juce::String& text, Icon icon, std::function<void()> on_click);

    /// Clears all items of MoreOptionsMenu, leaving it empty.
    void clear();

    friend class MoreOptions;
};