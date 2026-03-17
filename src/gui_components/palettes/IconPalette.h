#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

typedef std::unique_ptr<juce::Drawable> Icon;

class IconPalette
{
public:
    static Icon Arrow(int size, juce::Colour color);
    static Icon Check(int size, juce::Colour color);
    static Icon Chevron(int size, juce::Colour color);
    static Icon Warning(int size, juce::Colour color);
    static Icon Attention(int size, juce::Colour color);
    static Icon Import(int size, juce::Colour color);
    static Icon Logo(int size, juce::Colour color);
    static Icon Play(int size, juce::Colour color);
    static Icon Record(int size, juce::Colour color);
    static Icon Save(int size, juce::Colour color);
    static Icon Stop(int size, juce::Colour color);
    static Icon MoreOptions(int size, juce::Colour color);
    static Icon Trash(int size, juce::Colour color);
private:
    static Icon create_icon(const char* data, int data_size, int icon_size, juce::Colour color);
    static void recolor(juce::Drawable* icon, const juce::Colour& color);
    static void resize(juce::Drawable* icon, int size);
};
