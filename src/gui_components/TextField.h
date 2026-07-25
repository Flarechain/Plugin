#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class TextField : public juce::TextEditor
{
public:
    /// Creates a new TextField.
    TextField();

    ~TextField() override = default;

    void paint(juce::Graphics& g) override;
    void focusLost(FocusChangeType cause) override;

    /// Called when the text changes and the user has finished editing.
    ///
    /// Unlike juce::TextEditor::onTextChange, this callback is triggered only
    /// after the user completes the interaction with the text field, typically
    /// when the component loses keyboard focus.
    std::function<void(juce::String)> on_change;

    void setText(const juce::String& newText, bool sendTextChangeMessage = true);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextField)

    juce::String current_text;
};