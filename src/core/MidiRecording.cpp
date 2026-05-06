#include "MidiRecording.h"

#include "MidiPlayback.h"

void MidiRecording::record(PatternId id)
{
    if (state == NotRecording)
    {
        state = Recording;
        recording_pattern = id;

        if (on_record) { on_record(id); }
    }
}

void MidiRecording::stop()
{
    if (state == Recording)
    {
        state = NotRecording;
        if (on_stop && recording_pattern) { on_stop(recording_pattern.value(), recording_midi); }
        reset();
    }
}

void MidiRecording::add_buffer(const juce::MidiBuffer& buffer, const int buffer_size)
{
    std::optional<juce::uint32> first_note_sample_position;
    for (auto event : buffer)
    {
        auto message = event.getMessage();
        const juce::uint32 timestamp_samples = first_note ? 0 : static_cast<juce::uint32>(event.samplePosition) + sample_offset;
        const auto timestamp_seconds = static_cast<double>(timestamp_samples) / sample_rate;

        if (timestamp_seconds > MAX_MIDI_DURATION_SECONDS)
        {
            stop();
            return;
        }

        if (MidiNormalizer::is_relevant_midi_message(message))
        {
            if (first_note)
            {
                first_note = false;
                first_note_sample_position = static_cast<juce::uint32>(event.samplePosition);
            }
            message.setTimeStamp(timestamp_seconds);
            recording_midi.addEvent(message, 0);
        }
    }

    if (first_note_sample_position) { sample_offset = static_cast<juce::uint32>(buffer_size) - first_note_sample_position.value(); }
    else if (!first_note) { sample_offset += static_cast<juce::uint32>(buffer_size); }
}

void MidiRecording::reset()
{
    recording_pattern = std::nullopt;
    recording_midi.clear();
    sample_offset = 0;
    first_note = true;
}
