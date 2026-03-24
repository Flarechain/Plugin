#include "Pattern.h"

Pattern::Pattern(PatternId id) : id(id)
{
    name = juce::String::charToString(static_cast<char>('A' + (id % 26)));
}