#pragma once

#include "Pattern.h"

class PatternList
{
public:
    PatternList(juce::uint8 num_patterns);
    ~PatternList() = default;

    // iterators
    auto begin() { return list.begin(); }
    auto end()   { return list.end(); }
    auto begin() const { return list.begin(); }
    auto end()   const { return list.end(); }

    [[nodiscard]] juce::uint8 length() const { return static_cast<juce::uint8>(list.size()); }
    [[nodiscard]] Pattern& get(PatternId pattern_id) const { return *list.at(pattern_id); }

private:
    std::vector<std::unique_ptr<Pattern>> list;
};