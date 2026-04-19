#pragma once

#include <juce_osc/juce_osc.h>

class OscParser
{
public:
    /// Parses a string into an OSC message consisting of an OSC address and optional arguments.
    ///
    /// The OSC address must always start with '/'.
    ///
    /// The supported argument types are int32, float32, and string.
    ///
    /// Boolean values are supported syntactically and are converted to int32 (0 or 1).
    /// Boolean parsing is case-insensitive: true/false, TRUE/FALSE are valid.
    ///
    /// If a token is wrapped in double quotes, it is treated as a single string argument,
    /// allowing spaces inside the value (e.g. "turn on").
    ///
    /// Note:
    /// - Unquoted "true" / "false" are parsed as booleans.
    /// - Quoted "true" is treated as a string.
    /// @code
    /// juce::OSCMessage osc = OscParser::parse("/address true 0.5 -6");
    /// osc = OscParser::parse("/address TRUE play \"turn on\"");
    /// @endcode
    static std::optional<juce::OSCMessage> parse(const juce::String& osc_message);
private:
    static std::optional<bool> get_bool(const juce::String& string);
    static std::optional<int> get_int(const juce::String& string);
    static std::optional<float> get_float(const juce::String& string);
};