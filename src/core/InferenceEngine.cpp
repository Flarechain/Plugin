#include "InferenceEngine.h"

InferenceEngine::InferenceEngine() : thread(*this), state(NotInferencing), threshold(0.0f),
    sequence_length(0), pitch_wheel(PitchWheel::Center)
{
    env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "InferenceEngine");

    options = Ort::SessionOptions();
    options.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    options.SetIntraOpNumThreads(1);
    options.SetInterOpNumThreads(1);

    thread.startThread();
}

InferenceEngine::~InferenceEngine()
{
    thread.stopThread(1000);
}

void InferenceEngine::start(juce::File model_file, float threshold)
{
    if (state == Inferencing) { return; }

    if (threshold <= 0.0f) { this->threshold = 0.0f; }
    else if (threshold >= 1.0f) { this->threshold = 1.0f; }
    else { this->threshold = threshold; }

    this->model_file = std::move(model_file);

    state = PendingModel;
}

void InferenceEngine::stop()
{
    state = NotInferencing;
    session.reset();
    pitch_buffer.clear();
    pitch_wheel = PitchWheel::Center;
    threshold = 0.0f;
    sequence_length = 0;
}

void InferenceEngine::push_buffer(const juce::MidiBuffer& buffer)
{
    for (const auto event : buffer)
    {
        const auto message = event.getMessage();
        if (message.isPitchWheel())
        {
            pitch_wheel = message.getPitchWheelValue();
        }
        else if (message.isNoteOn(false))
        {
            const auto pitch = get_pitch(message).value();
            pitch_buffer.push(pitch);
            break;
        }
        else if (message.isNoteOff(true))
        {
            pitch_buffer.push(0);
            break;
        }
    }
}

void InferenceEngine::InferenceThread::run()
{
    while (!threadShouldExit())
    {
        if (engine.state == PendingModel)
        {
            engine.session = std::make_unique<Ort::Session>(
                engine.env,
                engine.model_file.getFullPathName().toStdString().c_str(),
                engine.options
            );

            const auto input_shape = engine.session->GetInputTypeInfo(0)
                .GetTensorTypeAndShapeInfo()
                .GetShape();
            engine.sequence_length = static_cast<juce::uint8>(input_shape[1]);

            engine.state = Inferencing;
        }
        else if (engine.state == Inferencing && engine.pitch_buffer.count() >= engine.sequence_length)
        {
            std::vector<float> input(engine.sequence_length);
            for (int i = 0; i < engine.sequence_length; ++i)
            {
                float pitch;
                engine.pitch_buffer.pop(pitch);
                input[i] = pitch;
            }

            const std::array<int64_t, 2> input_shape = { 1, engine.sequence_length };
            constexpr std::array<int64_t, 1> output_shape = { 1 };

            int64_t result = 0;
            auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
            auto input_tensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), input.size(), input_shape.data(), input_shape.size());
            auto output_tensor = Ort::Value::CreateTensor<int64_t>(memory_info, &result, 1, output_shape.data(), output_shape.size());

            Ort::AllocatorWithDefaultOptions allocator;
            std::string input_name = engine.session->GetInputNameAllocated(0, allocator).get();
            std::string output_name = engine.session->GetOutputNameAllocated(0, allocator).get();
            const char* input_name_ptr = input_name.c_str();
            const char* output_name_ptr = output_name.c_str();

            try
            {
                engine.session->Run(Ort::RunOptions{nullptr}, &input_name_ptr, &input_tensor, 1, &output_name_ptr, &output_tensor, 1);
            }
            catch (const Ort::Exception& e)
            {
                juce::Logger::writeToLog("ORT error: " + juce::String(e.what()));
            }

            const juce::uint8 label = static_cast<juce::uint8>(result);
            if (label < 5)
            {
                if (engine.on_pattern_detected) { engine.on_pattern_detected(label); }
            }
        }

        sleep(50);
    }
}

std::optional<float> InferenceEngine::get_pitch(const juce::MidiMessage& message) const
{
    if (!message.isNoteOnOrOff()) { return std::nullopt; }

    const auto note = static_cast<float>(message.getNoteNumber());
    return note + (static_cast<float>(pitch_wheel) - PitchWheel::Center) / (PitchWheel::Center / 2.0f);
}
