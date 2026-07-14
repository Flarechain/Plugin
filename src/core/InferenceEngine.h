#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <onnxruntime_cxx_api.h>
#include "RealtimeBuffer.h"
#include "../model/Pattern.h"

typedef float Pitch;

namespace PitchWheel
{
    static constexpr int Min = 0;
    static constexpr int Center = 8192;
    static constexpr int Max = 16383;
}

class InferenceEngine
{
public:
    InferenceEngine(int num_patterns);
    ~InferenceEngine();

    void start(juce::File model_file, float threshold);

    void stop();

    void push_buffer(const juce::MidiBuffer& buffer);

    std::function<void(PatternId id)> on_pattern_detected;

private:
    enum State
    {
        NotInferencing,
        PendingModel,
        Inferencing
    };

    class InferenceThread : public juce::Thread
    {
    public:
        explicit InferenceThread(InferenceEngine& engine) : juce::Thread("Background inference thread"), engine(engine) {}
        ~InferenceThread() override = default;

        void run() override;

    private:
        InferenceEngine& engine;
    };

    const int num_patterns;
    InferenceThread thread;
    std::atomic<State> state;
    Ort::Env env;
    Ort::SessionOptions options;

    juce::File model_file;
    std::atomic<float> threshold;
    std::unique_ptr<Ort::Session> session;
    std::atomic<juce::uint8> sequence_length;

    RealtimeBuffer<Pitch, 128> pitch_buffer;
    std::atomic<int> pitch_wheel;

    std::optional<float> get_pitch(const juce::MidiMessage& message) const;
};