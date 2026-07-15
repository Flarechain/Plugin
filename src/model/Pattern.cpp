#include "Pattern.h"

Pattern::Pattern(PatternId id) : id(id)
{
    name = juce::String::charToString(static_cast<char>('A' + (id % 26)));
}

void Pattern::clear()
{
    midi = juce::MidiMessageSequence();
    event.clear();
}

juce::ValueTree Pattern::to_value_tree() const
{
    juce::ValueTree state("Pattern");
    state.setProperty("id", id, nullptr);
    state.setProperty("name", name, nullptr);

    if (midi.getNumEvents() > 0)
    {
        juce::ValueTree midi_state("Midi");
        for (int i = 0; i < midi.getNumEvents(); ++i)
        {
            const auto* midi_event = midi.getEventPointer(i);
            const auto& message = midi_event->message;

            juce::ValueTree event_state("MidiEvent");
            event_state.setProperty("timestamp", message.getTimeStamp(), nullptr);

            juce::MemoryBlock raw_data(message.getRawData(), message.getRawDataSize());
            event_state.setProperty("data", raw_data.toBase64Encoding(), nullptr);

            midi_state.appendChild(event_state, nullptr);
        }
        state.appendChild(midi_state, nullptr);
    }

    state.appendChild(event.to_value_tree(), nullptr);
    return state;
}

void Pattern::from_value_tree(const juce::ValueTree& state)
{
    if (!state.isValid()) return;

    id = static_cast<PatternId>(static_cast<int>(state.getProperty("id")));
    name = state.getProperty("name").toString();

    midi.clear();
    const auto midi_state = state.getChildWithName("Midi");
    if (midi_state.isValid())
    {
        for (int i = 0; i < midi_state.getNumChildren(); ++i)
        {
            const auto event_state = midi_state.getChild(i);
            const double timestamp = event_state.getProperty("timestamp");

            juce::MemoryBlock raw_data;
            raw_data.fromBase64Encoding(event_state.getProperty("data").toString());

            juce::MidiMessage message(raw_data.getData(), static_cast<int>(raw_data.getSize()), timestamp);
            midi.addEvent(message, 0);
        }
        midi.sort();
        midi.updateMatchedPairs();
    }

    event.from_value_tree(state.getChildWithName("Event"));
}