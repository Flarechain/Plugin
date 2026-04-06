#include "PatternList.h"

PatternList::PatternList(const juce::uint8 num_patterns)
{
    for (juce::uint8 i = 0; i < num_patterns; i++)
    {
        auto pattern = Pattern(i);
        list.push_back(std::make_unique<Pattern>(pattern));
    }
}