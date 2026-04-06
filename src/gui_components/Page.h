#pragma once

#include <juce_core/juce_core.h>

enum class Page
{
    PatternSetup,
    LiveDetection
};

inline juce::String toString(const Page page)
{
    switch (page)
    {
        case Page::PatternSetup:  return "Pattern Setup";
        case Page::LiveDetection:  return "Live Detection";
    }
    return "";
}