#pragma once

#include <juce_core/juce_core.h>

/// Represents a piece of text inside a LogMessage.
///
/// A LogText wraps a juce::String and adds presentation metadata,
/// such as whether the text should be highlighted.
///
/// This is mainly used to build LogMessage sequences with highlighted
/// and non-highlighted text fragments.
struct LogText
{
private:
    juce::String text;
    bool highlighted;

public:
    /// Creates a new LogText with the given string, not highlighted.
    ///
    /// To highlight this text, use highlight().
    template<typename T, typename = std::enable_if_t<std::is_constructible_v<juce::String, T>>>
    explicit LogText(T&& text) : text(juce::String(std::forward<T>(text))), highlighted(false) {}

    [[nodiscard]] const juce::String& get_text() const { return text; }
    [[nodiscard]] bool is_highlighted() const { return highlighted; }

    /// Highlights the LogText.
    LogText& highlight()
    {
        highlighted = true;
        return *this;
    }
};

/// Represents a log message composed of multiple LogText fragments.
///
/// A LogMessage is a sequence of LogText objects that can contain
/// highlighted and non-highlighted text segments.
///
/// LogText fragments can be chained using the << operator to build
/// formatted log messages:
/// @code
/// LogMessage message = LogText("This is the IP address: ") << LogText("192.0.0").highlight();
/// @endcode
struct LogMessage
{
    std::vector<LogText> message;

    /// Creates a new LogMessage with a single LogText.
    LogMessage(LogText text) { message.push_back(std::move(text)); }

    LogMessage& operator<<(LogText&& text)
    {
        message.push_back(std::move(text));
        return *this;
    }

    LogMessage& operator<<(const LogText& text)
    {
        message.push_back(text);
        return *this;
    }

    auto begin() { return message.begin(); }
    auto end() { return message.end(); }

    [[nodiscard]] auto begin() const { return message.begin(); }
    [[nodiscard]] auto end() const { return message.end(); }
};

inline LogMessage operator<<(LogText first, LogText second)
{
    LogMessage message(std::move(first));
    message << std::move(second);
    return message;
}