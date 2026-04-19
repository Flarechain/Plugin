# Flarechain Plugin

Flarechain is an audio plugin.

## Dependencies

The dependencies are automatically fetched by CMake via `cpm`.

### JUCE

JUCE is the main framework used throughout the project. It is widely regarded as the industry standard for audio plugin development,
providing a set of tools for building cross-platform audio software.

In this project, JUCE is responsible for handling core aspects such as MIDI processing,
plugin formats (VST3 and AU), as well as the entire graphical user interface.
The standalone version of the plugin is primarily intended for UI debugging and development.

Within a Digital Audio Workstation (DAW), the plugin operates as a MIDI effect (or MIDI processor):
it receives MIDI input data, processes it, and forwards it to downstream instruments (e.g. synthesizer plugins)
without generating audio on its own. As a result, the actual sound generation is handled by external plugins,
while this plugin focuses exclusively on recognizing musical patterns in live performances and triggering events.

### Catch2

Catch2 is used as the unit testing framework for the project. It provides a lightweight and expressive way to define
and organize tests directly in C++, without requiring extensive boilerplate code.\
Catch2 is primarily used to validate core logic, such as parsing functionality and model-related behavior.