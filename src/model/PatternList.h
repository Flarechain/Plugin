#pragma once

#include <juce_data_structures/juce_data_structures.h>

#include "Pattern.h"

class PatternList
{
public:
    explicit PatternList(juce::uint8 num_patterns);
    ~PatternList() = default;

    // iterators
    auto begin() { return list.begin(); }
    auto end()   { return list.end(); }
    [[nodiscard]] auto begin() const { return list.begin(); }
    [[nodiscard]] auto end()   const { return list.end(); }

    [[nodiscard]] juce::uint8 length() const { return static_cast<juce::uint8>(list.size()); }
    [[nodiscard]] Pattern& get(const PatternId pattern_id) const { return *list.at(pattern_id); }

    /// Serializes the PatternList to a ValueTree.
    juce::ValueTree to_value_tree() const;

    /// Restores the PatternList from a ValueTree.
    void from_value_tree(const juce::ValueTree& state);

private:
    std::vector<std::unique_ptr<Pattern>> list;
};