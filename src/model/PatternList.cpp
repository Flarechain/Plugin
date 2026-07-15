#include "PatternList.h"

PatternList::PatternList(const juce::uint8 num_patterns)
{
    for (juce::uint8 i = 0; i < num_patterns; i++)
    {
        auto pattern = Pattern(i);
        list.push_back(std::make_unique<Pattern>(pattern));
    }
}

juce::ValueTree PatternList::to_value_tree() const
{
    juce::ValueTree state("PatternList");
    for (const auto& pattern : list)
    {
        state.appendChild(pattern->to_value_tree(), nullptr);
    }
    return state;
}

void PatternList::from_value_tree(const juce::ValueTree& state)
{
    if (!state.isValid()) return;

    list.clear();
    for (int i = 0; i < state.getNumChildren(); ++i)
    {
        auto pattern = std::make_unique<Pattern>(i);
        pattern->from_value_tree(state.getChild(i));
        list.push_back(std::move(pattern));
    }
}