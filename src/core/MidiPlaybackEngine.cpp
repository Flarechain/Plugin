#include "MidiPlaybackEngine.h"

void MidiPlaybackEngine::play(PatternId id, juce::MidiMessageSequence midi)
{
    if (is_playing())
    {
        stop();
        pending_pattern = id;
        pending_midi = std::move(midi);
    }
    else
    {
        state = Playing;
        playing_pattern = id;
        this->playing_midi = std::move(midi);
    }

    if (on_play) { on_play(id); }
}

void MidiPlaybackEngine::stop()
{
    if (is_playing())
    {
        state = Stopping;
        if (on_stop && playing_pattern) { on_stop(playing_pattern.value()); }
        reset_playing();
    }
}

juce::MidiBuffer MidiPlaybackEngine::get_next_buffer(const int buffer_size)
{
    auto buffer = juce::MidiBuffer();

    if (state == Stopping)  // if playback has to stop, send the last buffer turning off notes and controls
    {
        for (int channel = 1; channel <= 16; channel++)
        {
            // turn off active notes
            for (int note = 0; note < 128; note++)
            {
                const int channel_index = channel - 1;
                if (active_notes[channel_index][note])
                {
                    buffer.addEvent(juce::MidiMessage::noteOff(channel, note, 0.0f), 0);
                    active_notes[channel_index][note] = false;
                }
            }

            // reset controls
            buffer.addEvent(juce::MidiMessage::controllerEvent(channel, 64, 0), 0); // sustain pedal
            buffer.addEvent(juce::MidiMessage::controllerEvent(channel, 66, 0), 0); // sostenuto pedal
            buffer.addEvent(juce::MidiMessage::controllerEvent(channel, 67, 0), 0); // soft pedal
            buffer.addEvent(juce::MidiMessage::controllerEvent(channel, 1, 0), 0); // modulation wheel

            buffer.addEvent(juce::MidiMessage::pitchWheel(channel, 8192), 0);
        }

        if (pending_pattern) { pending_to_playing(); }
        else { state = NotPlaying; }

        return buffer;
    }

    if (midi_index >= playing_midi.getNumEvents())
    {
        stop();
        return buffer;
    }

    while (midi_index < playing_midi.getNumEvents())
    {
        const auto message = playing_midi.getEventPointer(midi_index)->message;
        const auto timestamp_seconds = message.getTimeStamp();
        const int timestamp_samples = static_cast<int>(std::round(timestamp_seconds * sample_rate));

        const int sample_index = static_cast<int>(static_cast<juce::uint32>(timestamp_samples) - sample_offset);
        if (sample_index >= 0 && sample_index < buffer_size)
        {
            const int channel_index = message.getChannel() - 1;
            if (message.isNoteOn()) { active_notes[channel_index][message.getNoteNumber()] = true; }
            else if (message.isNoteOff()) { active_notes[channel_index][message.getNoteNumber()] = false; }

            buffer.addEvent(message, sample_index);
        }
        else { break; }

        ++midi_index;
    }

    sample_offset += static_cast<juce::uint32>(buffer_size);
    return buffer;
}

void MidiPlaybackEngine::reset_playing()
{
    playing_pattern = std::nullopt;
    playing_midi.clear();
    midi_index = 0;
    sample_offset = 0;
}

void MidiPlaybackEngine::pending_to_playing()
{
    state = Playing;
    playing_pattern = pending_pattern;
    playing_midi = std::move(pending_midi);
    pending_pattern = std::nullopt;
    pending_midi = juce::MidiMessageSequence();
}
