#include "IconPalette.h"
#include "BinaryData.h"

// TODO: docs

std::unique_ptr<juce::Drawable> IconPalette::Arrow(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::arrow_svg, BinaryData::arrow_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Check(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::check_svg, BinaryData::check_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Chevron(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::chevrondown_svg, BinaryData::chevrondown_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Warning(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::exclamationcircle_svg, BinaryData::exclamationcircle_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Import(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::import_svg, BinaryData::import_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Logo(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::logo_extended_svg, BinaryData::logo_extended_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Play(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::play_svg, BinaryData::play_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Record(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::record_svg, BinaryData::record_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Save(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::save_svg, BinaryData::save_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Stop(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::stop_svg, BinaryData::stop_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::MoreOptions(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::threedots_svg, BinaryData::threedots_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::Trash(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::trash_svg, BinaryData::trash_svgSize, size, color);
}

std::unique_ptr<juce::Drawable> IconPalette::create_icon(const char* data, const int data_size, const int icon_size, const juce::Colour color)
{
    auto icon = juce::Drawable::createFromImageData(data, static_cast<size_t>(data_size));

    recolor(icon.get(), color);
    resize(icon.get(), icon_size);

    return icon;
}

void IconPalette::recolor(juce::Drawable* icon, const juce::Colour& color)
{
    icon->replaceColour (juce::Colours::black, color);
}

void IconPalette::resize(juce::Drawable* icon, const int size)
{
    const float old_width = icon->getDrawableBounds().getWidth();   // icon old width
    const float old_height = icon->getDrawableBounds().getHeight(); // icon old height
    const float scale = static_cast<float>(size) / old_height;
    const float new_width = old_width * scale;      // icon new width
    const float new_height = old_height * scale;    // icon new height
    icon->setTransform(juce::AffineTransform::scale(scale, scale));

    const int bounds_width = static_cast<int>(std::ceil(new_width));    // icon bounds new width
    const int bounds_height = static_cast<int>(std::ceil(new_height));  // icon bounds new height
    icon->setBounds(0, 0, bounds_width, bounds_height);
}