#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

typedef std::unique_ptr<juce::Drawable> Icon;

class IconPalette
{
public:
    /// Returns an Arrow icon with the given size and color.
    static Icon Arrow(int size, juce::Colour color);

    /// Returns a Check icon with the given size and color.
    static Icon Check(int size, juce::Colour color);

    /// Returns a Chevron icon with the given size and color.
    static Icon Chevron(int size, juce::Colour color);

    /// Returns a Warning icon with the given size and color.
    static Icon Warning(int size, juce::Colour color);

    /// Returns an Attention icon with the given size and color.
    static Icon Attention(int size, juce::Colour color);

    /// Returns an Import icon with the given size and color.
    static Icon Import(int size, juce::Colour color);

    /// Returns the extended Flarechain logo with the given size and color.
    static Icon Logo(int size, juce::Colour color);

    /// Returns a Play icon with the given size and color.
    static Icon Play(int size, juce::Colour color);

    /// Returns a Record icon with the given size and color.
    static Icon Record(int size, juce::Colour color);

    /// Returns a Save icon with the given size and color.
    static Icon Save(int size, juce::Colour color);

    /// Returns a Stop icon with the given size and color.
    static Icon Stop(int size, juce::Colour color);

    /// Returns a More Options icon with the given size and color.
    static Icon MoreOptions(int size, juce::Colour color);

    /// Returns a Trash icon with the given size and color.
    static Icon Trash(int size, juce::Colour color);

private:
    /// Creates an icon as a unique pointer of juce::Drawable.
    static Icon create_icon(const char* data, int data_size, int icon_size, juce::Colour color);

    /// Recolors the icon with a new color.
    static void recolor(juce::Drawable* icon, const juce::Colour& color);

    /// Resizes the icon proportionally with a new height size.
    static void resize(juce::Drawable* icon, int size);
};
