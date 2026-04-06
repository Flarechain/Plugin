#include "IconPalette.h"
#include "BinaryData.h"

Icon IconPalette::Arrow(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::arrow_svg, BinaryData::arrow_svgSize, size, color);
}

Icon IconPalette::Check(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::check_svg, BinaryData::check_svgSize, size, color);
}

Icon IconPalette::Chevron(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::chevrondown_svg, BinaryData::chevrondown_svgSize, size, color);
}

Icon IconPalette::Warning(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::warning_svg, BinaryData::warning_svgSize, size, color);
}

Icon IconPalette::Attention(int size, juce::Colour color)
{
    return create_icon(BinaryData::exclamationcircle_svg, BinaryData::exclamationcircle_svgSize, size, color);
}

Icon IconPalette::Import(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::import_svg, BinaryData::import_svgSize, size, color);
}

Icon IconPalette::Logo(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::logo_extended_svg, BinaryData::logo_extended_svgSize, size, color);
}

Icon IconPalette::Play(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::play_svg, BinaryData::play_svgSize, size, color);
}

Icon IconPalette::Record(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::record_svg, BinaryData::record_svgSize, size, color);
}

Icon IconPalette::Save(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::save_svg, BinaryData::save_svgSize, size, color);
}

Icon IconPalette::Stop(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::stop_svg, BinaryData::stop_svgSize, size, color);
}

Icon IconPalette::MoreOptions(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::threedots_svg, BinaryData::threedots_svgSize, size, color);
}

Icon IconPalette::Trash(const int size, const juce::Colour color)
{
    return create_icon(BinaryData::trash_svg, BinaryData::trash_svgSize, size, color);
}

Icon IconPalette::create_icon(const char* data, const int data_size, const int icon_size, const juce::Colour color)
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
    const float viewbox_ratio = icon->getLocalBounds().toFloat().getAspectRatio(true); // ratio as width / height
    // const float old_viewbox_width = icon->getLocalBounds().toFloat().getWidth();
    const float old_viewbox_height = icon->getLocalBounds().toFloat().getHeight();
    const auto new_viewbox_height = static_cast<float>(size);
    const float new_viewbox_width = new_viewbox_height * viewbox_ratio;

    const float drawable_ratio = icon->getDrawableBounds().toFloat().getAspectRatio(true);
    // const auto old_drawable_width = icon->getDrawableBounds().getWidth();
    const auto old_drawable_height = icon->getDrawableBounds().getHeight();
    const auto new_drawable_height = (new_viewbox_height * old_drawable_height) / old_viewbox_height;
    const auto new_drawable_width = new_drawable_height * drawable_ratio;

    icon->setBounds(0, 0,
        static_cast<int>(std::ceil(new_viewbox_width)),
        static_cast<int>(std::ceil(new_viewbox_height))
    );

    auto new_drawable_bounds = juce::Rectangle<float>(0,
        0,
        new_drawable_width,
        new_drawable_height
    );
    juce::RectanglePlacement placement { juce::RectanglePlacement::centred };
    new_drawable_bounds = placement.appliedTo(new_drawable_bounds, icon->getLocalBounds().toFloat());

    icon->setTransformToFit(new_drawable_bounds, juce::RectanglePlacement::stretchToFit);
}