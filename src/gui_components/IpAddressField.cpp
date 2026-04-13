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

    point_1.setFont(font);
    point_1.setText(".", juce::dontSendNotification);
    point_1.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    point_1.setSize(4, static_cast<int>(font.getHeight()));
    point_1.setMinimumHorizontalScale(1.0f);
    point_1.setBorderSize(juce::BorderSize(0));

    point_2.setFont(font);
    point_2.setText(".", juce::dontSendNotification);
    point_2.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    point_2.setSize(4, static_cast<int>(font.getHeight()));
    point_2.setMinimumHorizontalScale(1.0f);
    point_2.setBorderSize(juce::BorderSize(0));

    point_3.setFont(font);
    point_3.setText(".", juce::dontSendNotification);
    point_3.setColour(juce::Label::textColourId, ColorPalette::Coffee500);
    point_3.setSize(4, static_cast<int>(font.getHeight()));
    point_3.setMinimumHorizontalScale(1.0f);
    point_3.setBorderSize(juce::BorderSize(0));

    setSize(192, block_field_1.getHeight());

    addAndMakeVisible(ip_label);
    addAndMakeVisible(block_field_1);
    addAndMakeVisible(block_field_2);
    addAndMakeVisible(block_field_3);
    addAndMakeVisible(block_field_4);
    addAndMakeVisible(point_1);
    addAndMakeVisible(point_2);
    addAndMakeVisible(point_3);
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
        static_cast<float>(block_field_1.getWidth()),
        static_cast<float>(block_field_1.getHeight()),
        block_field_1)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(point_1.getWidth()),
        static_cast<float>(point_1.getHeight()),
        point_1)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(block_field_2.getWidth()),
        static_cast<float>(block_field_2.getHeight()),
        block_field_2)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(point_2.getWidth()),
        static_cast<float>(point_2.getHeight()),
        point_2)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(block_field_3.getWidth()),
        static_cast<float>(block_field_3.getHeight()),
        block_field_3)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(point_3.getWidth()),
        static_cast<float>(point_3.getHeight()),
        point_3)
    );
    fb.items.add(juce::FlexItem(
        static_cast<float>(block_field_4.getWidth()),
        static_cast<float>(block_field_4.getHeight()),
        block_field_4)
    );

    int gap = 2;
    auto ip_field_area = juce::Rectangle(
        block_field_1.getWidth() * 4 + gap * 6 + point_1.getWidth() * 3,
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
    block_field_1.clear();
    block_field_2.clear();
    block_field_3.clear();
    block_field_4.clear();
}