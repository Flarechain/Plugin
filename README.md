# Flarechain Plugin

Flarechain is a cross-platform audio plugin that enables real-time musical pattern recognition and OSC event triggering
during live performances.
It allows musicians to associate short MIDI patterns with custom actions, enabling expressive control of lighting systems,
visual effects, augmented and virtual reality experiences, game engines, and any other OSC-enabled application.

This project focuses on custom UI development and the implementation of the plugin core functionalities.
This also includes the integration of an AI model, but the development, training, and evaluation of the model are outside
the scope of this project. For more information about the AI model, you can visit the [related repository](https://github.com/ns2max/hot_licks).


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

### ONNX Runtime

ONNX Runtime is a cross-platform inference engine developed by Microsoft for executing machine learning models
in the ONNX format. It provides optimized execution of neural networks across different hardware platforms.\
In this project, ONNX Runtime is used to load and execute the pre-trained AI model responsible for the
real-time musical pattern recognition.


## Functionalities

### MIDI Pattern Management

Flarechain allows users to create and manage custom musical patterns that can be used as triggers for external events.
Patterns can be recorded directly from MIDI input or imported from existing MIDI files.
Each pattern is normalized, setting a maximum duration of `10 seconds` and only allowing the following MIDI messages:
`NoteOn`, `NoteOff`, `PitchWheel`, `Aftertouch`, `ModulationWheel`, `SustainPedalOn`, `SustainPedalOff`,
`SostenutoPedalOn`, `SostenutoPedalOff`, `SoftPedalOn`, `SoftPedalOff`.

### OSC Communication

Once a musical pattern is recognized, Flarechain can send an OSC message to an external application or device.
Each pattern can be associated with a custom OSC message and a destination IP address.

#### OSC message

An OSC message consists of an address pattern and an optional list of arguments.
The user can define the desired OSC message as a text string, which is then parsed and converted into a valid OSC message.
The OSC address must always start with `/`. Arguments can be optionally appended after the address and are interpreted
according to their type.

The supported argument types are:

- **Integer values** (`int32`)

- **Floating-point values** (`float32`)

- **String values**

Boolean values are also supported syntactically and are automatically converted into integer values (`true` → `1`, `false` → `0`).
Boolean parsing is case-insensitive, meaning that values such as `true`, `TRUE`, `false`, and `FALSE` are considered valid.\
String arguments containing spaces must be enclosed in double quotes. This allows multi-word values to be transmitted
as a single OSC argument. For example, `"turn on"` is interpreted as one string argument, while `turn on` would be
interpreted as two separate arguments.\
It is important to note that quoted boolean values are treated as strings. For example, `true` is converted to the
integer value `1`, while `"true"` is transmitted as the string `"true"`.

Examples of valid OSC messages:

```
/lights/spotlight 255 100 50
/effects/reverb true
/scene/change intro
/pattern/detected "pattern A"
/visual/particles/intensity 0.8
```

#### IP Address

The IP address is the one defined by the user, with a fixed port of `7700`.

### Live Detection

Live Detection allows Flarechain to recognize the MIDI patterns during a performance and automatically trigger
the associated OSC events.\
The most recently detected patterns are displayed directly in the UI, providing immediate visual feedback
during a performance. Each detected event is also recorded in a log file, which contains more detailed information
about the detection history and can be accessed directly through the dedicated button in the interface.

This project currently uses a static pre-trained AI model for prototyping and demonstration purposes.
The model is not directly connected to the patterns defined by the user inside the plugin.
Future implementations will introduce a cloud-based training pipeline, allowing users to train a customized model
using their own patterns. Once trained, the model will be downloaded and deployed locally for real-time inference
inside the plugin.

The model can currently be replaced manually by placing a compatible ONNX file inside the `ai_models` directory
and naming it `default.onnx`.
To be compatible with the inference engine, the model must use the following input and output specifications:
- **Input shape:** `[1, n]` with `float` values, representing a sequence of `n` pitch note values.
- **Output shape:** `[1]` with an integer value, representing the predicted pattern class (0 to 4 for patterns 0-4,
5 for "no pattern").