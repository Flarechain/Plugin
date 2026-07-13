#include "OscParser.h"

std::optional<juce::OSCMessage> OscParser::parse(const juce::String& osc_message)
{
    if (osc_message.isEmpty()) return std::nullopt;

    juce::StringArray tokens;
    tokens.addTokens(osc_message, " ", "\"");
    tokens.removeEmptyStrings();
    if (tokens.size() == 0) return std::nullopt;

    const juce::String address = tokens[0];
    if (!address.startsWith("/")) return std::nullopt;

    juce::OSCMessage message(address);

    for (int i = 1; i < tokens.size(); ++i)
    {
        const juce::String& token = tokens[i].unquoted();

        if (auto bool_token = get_bool(token))
        {
            message.addArgument(bool_token.value());
        }
        else if (auto int_token = get_int(token))
        {
            message.addArgument(int_token.value());
        }
        else if (auto float_token = get_float(token))
        {
            message.addArgument(float_token.value());
        }
        else
        {
            message.addArgument(token);
        }
    }

    return message;
}

std::optional<bool> OscParser::get_bool(const juce::String& string)
{
    if (string.isEmpty()) return std::nullopt;

    if (string == "true" || string == "TRUE")   return true;
    if (string == "false" || string == "FALSE") return false;

    return std::nullopt;
}

std::optional<int> OscParser::get_int(const juce::String& string)
{
    if (string.isEmpty()) return std::nullopt;

    int start = 0;
    bool negative = false;

    if (string.startsWithChar('-'))
    {
        if (string.length() == 1) return std::nullopt;

        negative = true;
        start = 1;
    }

    for (int i = start; i < string.length(); ++i)
    {
        if (! juce::CharacterFunctions::isDigit(string[i])) return std::nullopt;
    }

    int value = string.substring(start).getIntValue();
    return negative ? -value : value;
}

std::optional<float> OscParser::get_float(const juce::String& string)
{
    if (string.isEmpty()) return std::nullopt;

    int start = 0;
    bool negative = false;

    if (string.startsWithChar('-'))
    {
        if (string.length() == 1) return std::nullopt;

        negative = true;
        start = 1;
    }

    bool dot_seen = false;
    for (int i = start; i < string.length(); ++i)
    {
        const auto c = string[i];

        if (c == '.')
        {
            if (dot_seen) return std::nullopt;

            dot_seen = true;
        }
        else if (! juce::CharacterFunctions::isDigit(c)) return std::nullopt;
    }

    if (string.startsWithChar('.')) return std::nullopt;
    if (string.endsWithChar('.')) return std::nullopt;

    float value = string.substring(start).getFloatValue();
    return negative ? -value : value;
}

juce::String toString(const juce::OSCMessage& osc_message)
{
    juce::String string;
    string << osc_message.getAddressPattern().toString();

    for (int i = 0; i < osc_message.size(); ++i)
    {
        const auto& arg = osc_message[i];
        if (arg.isInt32()) { string << " " << arg.getInt32(); }
        else if (arg.isFloat32()) { string << " " << arg.getFloat32(); }
        else if (arg.isString()) { string << " " << arg.getString(); }
    }

    return string;
}