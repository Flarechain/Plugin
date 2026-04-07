#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "palettes/IconPalette.h"

class IpAddressField : public juce::Component
{
public:
    /// Creates a new IpAddressField.
    IpAddressField();

    ~IpAddressField() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IpAddressField)

    /// IpAddressField's subcomponent for displaying a single block field of the IP address.
    class IpBlockField : public juce::TextEditor
    {
    public:
        IpBlockField();

        ~IpBlockField() override = default;

        void paint(juce::Graphics& g) override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IpBlockField)
    };

    Icon icon;
    juce::Label ip_label;
    IpBlockField block_field_1, block_field_2, block_field_3, block_field_4;
    juce::Label point_1, point_2, point_3;
};