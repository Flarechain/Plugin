#include "IpAddressField.h"
#include "palettes/Palette.h"

IpAddressField::IpAddressField()
{
    setInterceptsMouseClicks(false, true);

    icon = IconPalette::Arrow(9, ColorPalette::Coffee500);

    const auto font = FontPalette::Text_S_Semibold;
    ip_label.setFont(font);
    ip_label.setText("IP:", juce::dontSendNotification);
    ip_label.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    ip_label.setSize(16, static_cast<int>(font.getHeight()));
    ip_label.setMinimumHorizontalScale(1.0f); // disable font stretching
    ip_label.setBorderSize(juce::BorderSize(0));
    ip_label.setInterceptsMouseClicks(false, true);

    for (juce::uint8 i = 0; i < point_labels.size(); ++i)
    {
        point_labels[i].setFont(font);
        point_labels[i].setText(".", juce::dontSendNotification);
        point_labels[i].setColour(juce::Label::textColourId, ColorPalette::Coffee500);
        point_labels[i].setSize(4, static_cast<int>(font.getHeight()));
        point_labels[i].setMinimumHorizontalScale(1.0f);
        point_labels[i].setBorderSize(juce::BorderSize(0));
    }

    for (juce::uint8 i = 0; i < block_fields.size(); ++i)
    {
        block_fields[i].on_change = [this, i](const std::optional<juce::uint8> value)
        {
            blocks[i] = value;
            send_ip_change();
        };
        block_fields[i].on_arrow_key = [this, i](const juce::KeyPress& key) { handle_arrow_keys(key, i); };
    }

    setSize(192, block_fields[0].getHeight());

    addAndMakeVisible(ip_label);
    for (auto& block_field : block_fields) { addAndMakeVisible(block_field); }
    for (auto& point_label : point_labels) { addAndMakeVisible(point_label); }
}

void IpAddressField::paint(juce::Graphics& g)
{
    icon->drawWithin(g,
        getLocalBounds().toFloat(),
        juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize,
        1.0f
    );
}

void IpAddressField::resized()
{
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    fb.alignItems = juce::FlexBox::AlignItems::center;

    fb.items.add(juce::FlexItem(
        static_cast<float>(block_fields[0].getWidth()),
        static_cast<float>(block_fields[0].getHeight()),
        block_fields[0])
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(point_labels[0].getWidth()),
        static_cast<float>(point_labels[0].getHeight()),
        point_labels[0])
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(block_fields[1].getWidth()),
        static_cast<float>(block_fields[1].getHeight()),
        block_fields[1])
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(point_labels[1].getWidth()),
        static_cast<float>(point_labels[1].getHeight()),
        point_labels[1])
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(block_fields[2].getWidth()),
        static_cast<float>(block_fields[2].getHeight()),
        block_fields[2])
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(point_labels[2].getWidth()),
        static_cast<float>(point_labels[2].getHeight()),
        point_labels[2])
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(block_fields[3].getWidth()),
        static_cast<float>(block_fields[3].getHeight()),
        block_fields[3])
    );

    int gap = 2;
    auto ip_field_area = juce::Rectangle(
        block_fields[0].getWidth() * 4 + gap * 6 + point_labels[0].getWidth() * 3,
        getHeight()
    );
    const juce::RectanglePlacement ip_field_placement { juce::RectanglePlacement::xRight | juce::RectanglePlacement::yMid | juce::RectanglePlacement::doNotResize };
    ip_field_area = ip_field_placement.appliedTo(ip_field_area, getLocalBounds());

    fb.performLayout(ip_field_area.toFloat());

    gap = 4;
    const auto ip_label_area = juce::Rectangle<int>(
        ip_field_area.getX() - gap - ip_label.getWidth(),
        ip_field_area.getY(),
        ip_label.getWidth(),
        getHeight()
    );
    ip_label.setBounds(ip_label_area);
}

void IpAddressField::clear()
{
    block_fields[0].clear();
    block_fields[1].clear();
    block_fields[2].clear();
    block_fields[3].clear();
}

void IpAddressField::send_ip_change() const
{
    if (on_change)
    {
        std::optional<juce::IPAddress> ip = std::nullopt;
        if (blocks[0] && blocks[1] && blocks[2] && blocks[3])
        {
            ip = juce::IPAddress(blocks[0].value(), blocks[1].value(), blocks[2].value(), blocks[3].value());
        }
        on_change(ip);
    }
}

void IpAddressField::handle_arrow_keys(const juce::KeyPress& key, const juce::uint8 block_field_index)
{
    if (key == juce::KeyPress::rightKey)
    {
        if (block_field_index < block_fields.size() - 1)
        {
            block_fields[block_field_index + 1].grabKeyboardFocus();
        }
    }
    else if (key == juce::KeyPress::leftKey)
    {
        if (block_field_index > 0)
        {
            block_fields[block_field_index - 1].grabKeyboardFocus();
        }
    }
}