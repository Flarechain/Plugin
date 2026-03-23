#pragma once

#include <juce_core/juce_core.h>

struct LogText
{
private:
    juce::String text;
    bool highlighted;

public:
    template<typename T, typename = std::enable_if_t<std::is_constructible_v<juce::String, T>>>
    LogText(T&& text) : text(juce::String(std::forward<T>(text))), highlighted(false) {}

    const juce::String& get_text() const { return text; }
    bool is_highlighted() const { return highlighted; }

    LogText& highlight()
    {
        highlighted = true;
        return *this;
    }
};

struct LogMessage
{
    std::vector<LogText> message;

    LogMessage(LogText text) { message.push_back(std::move(text)); }

    LogMessage& operator<<(const LogText& text)
    {
        message.push_back(text);
        return *this;
    }

    auto begin() { return message.begin(); }
    auto end() { return message.end(); }

    auto begin() const { return message.begin(); }
    auto end() const { return message.end(); }
};

inline LogMessage operator<<(LogText first, LogText second)
{
    LogMessage message(std::move(first));
    message << std::move(second);
    return message;
}