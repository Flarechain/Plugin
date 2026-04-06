#include "FontPalette.h"

#include "BinaryData.h"

namespace
{
    juce::Typeface::Ptr get_sans_medium()
    {
        static auto font_sans_medium = juce::Typeface::createSystemTypefaceFor(
            BinaryData::GeneralSansMedium_otf,
            BinaryData::GeneralSansMedium_otfSize
        );
        return font_sans_medium;
    }

    juce::Typeface::Ptr get_sans_semibold()
    {
        static auto font_sans_semibold = juce::Typeface::createSystemTypefaceFor(
            BinaryData::GeneralSansSemibold_otf,
            BinaryData::GeneralSansSemibold_otfSize
        );
        return font_sans_semibold;
    }
}

const auto FontPalette::Text_XS_Medium = juce::Font(juce::FontOptions(get_sans_medium()).withPointHeight(11));
const auto FontPalette::Text_S_Medium = juce::Font(juce::FontOptions(get_sans_medium()).withPointHeight(12));
const auto FontPalette::Text_S_Semibold = juce::Font(juce::FontOptions(get_sans_semibold()).withPointHeight(12));
const auto FontPalette::Text_M_Medium = juce::Font(juce::FontOptions(get_sans_medium()).withPointHeight(14));
const auto FontPalette::Text_L_Semibold = juce::Font(juce::FontOptions(get_sans_semibold()).withPointHeight(16));
const auto FontPalette::Text_XL_Semibold = juce::Font(juce::FontOptions(get_sans_semibold()).withPointHeight(18));