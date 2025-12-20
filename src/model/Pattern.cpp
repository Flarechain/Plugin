#include "Pattern.h"

Pattern::Pattern(juce::uint8 id)
{
    this->id = id;
}

juce::uint8 Pattern::get_id() const
{
    return id;
}
