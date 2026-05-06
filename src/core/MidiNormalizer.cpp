#include "MidiNormalizer.h"

void MidiNormalizer::normalize(juce::MidiMessageSequence& midi) const
{
    if (midi.getNumEvents() == 0) return;

    midi.sort();
    midi.updateMatchedPairs();

    juce::MidiMessageSequence normalized_midi;

    double offset = 0;
    bool first = true;

    for (int i = 0; i < midi.getNumEvents(); ++i)
    {
        const auto* event = midi.getEventPointer(i);
        if (is_relevant_midi_message(event->message))
        {
            if (first)
            {
                offset = event->message.getTimeStamp();
                first = false;
            }

            if (event->message.getTimeStamp() - offset > MAX_MIDI_DURATION_SECONDS) { break; }
            normalized_midi.addEvent(event->message, -offset);
        }
    }

    normalized_midi.updateMatchedPairs();
    midi = normalized_midi;
}

bool MidiNormalizer::is_relevant_midi_message(const juce::MidiMessage& message)
{
    return message.isNoteOn(true) || message.isNoteOff(true) ||
        message.isPitchWheel() || message.isAftertouch() || message.isControllerOfType(1) ||
        message.isSustainPedalOn() || message.isSustainPedalOff() ||
        message.isSostenutoPedalOn() || message.isSostenutoPedalOff() ||
        message.isSoftPedalOn() || message.isSoftPedalOff();
}
