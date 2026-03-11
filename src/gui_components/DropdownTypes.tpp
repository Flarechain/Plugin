#pragma once

#include <type_traits>
#include <juce_gui_basics/juce_gui_basics.h>

// dropdown item's id must be an enum or int
template<
    typename T,
    typename = std::enable_if_t<
        std::is_enum_v<T> ||
        std::is_integral_v<T>
    >
>

struct Item
{
    T id;
    juce::String label;

    bool operator==(const Item& other) const
    {
        return id == other.id;
    }
};