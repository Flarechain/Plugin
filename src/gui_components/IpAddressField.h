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

    void set_ip(const juce::IPAddress& ip_address);

    /// Clears all fields of the IP address.
    void clear();

    /// Called when the IP address input changes.
    ///
    /// @param ip An optional IP address:
    /// - `std::nullopt` if the input provided by the user is incomplete
    /// - a valid `juce::IPAddress` if all fields are filled
    std::function<void(std::optional<juce::IPAddress> ip)> on_change;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IpAddressField)

    /// IpAddressField's subcomponent for displaying a single block field of the IP address.
    class IpBlockField : public juce::TextEditor
    {
    public:
        IpBlockField();

        ~IpBlockField() override = default;

        void paint(juce::Graphics& g) override;
        void focusLost(FocusChangeType cause) override;
        bool keyPressed(const juce::KeyPress& key) override;

        std::function<void(std::optional<juce::uint8>)> on_change;
        std::function<void(const juce::KeyPress&)> on_arrow_key;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IpBlockField)

        std::optional<juce::uint8> current_value;
    };

    Icon icon;
    juce::Label ip_label;
    std::array<IpBlockField, 4> block_fields;
    std::array<juce::Label, 3> point_labels;

    std::array<std::optional<juce::uint8>, 4> blocks;   // octets of the IP address

    void send_ip_change() const;

    /// Handles navigation between IP block fields using arrow keys.
    void handle_arrow_keys(const juce::KeyPress& key, juce::uint8 block_field_index);
};