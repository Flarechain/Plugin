#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class IconPalette
{
public:
    static std::unique_ptr<juce::Drawable> Arrow(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Check(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Chevron(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Warning(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Import(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Logo(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Play(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Record(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Save(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Stop(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> MoreOptions(int size, juce::Colour color);
    static std::unique_ptr<juce::Drawable> Trash(int size, juce::Colour color);
private:
    static std::unique_ptr<juce::Drawable> create_icon(const char* data, int data_size, int icon_size, juce::Colour color);
    static void recolor(juce::Drawable* icon, const juce::Colour& color);
    static void resize(juce::Drawable* icon, int size);
};
