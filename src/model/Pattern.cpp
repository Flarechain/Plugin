#include "Pattern.h"

Pattern::Pattern(PatternId id) : id(id)
{
    name = juce::String::charToString(static_cast<char>('A' + (id % 26)));
}

void Pattern::clear()
{
    midi = juce::MidiMessageSequence();
    event.clear();
}
