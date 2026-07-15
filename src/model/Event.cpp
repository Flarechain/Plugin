#include "Event.h"

void Event::clear()
{
    ip_address = std::nullopt;
    osc_message = std::nullopt;
}

juce::ValueTree Event::to_value_tree() const
{
    juce::ValueTree state("Event");

    if (ip_address) state.setProperty("ip", ip_address->toString(), nullptr);

    if (osc_message)
    {
        juce::ValueTree osc_state("OscMessage");
        osc_state.setProperty("address", osc_message->getAddressPattern().toString(), nullptr);

        for (int i = 0; i < osc_message->size(); ++i)
        {
            const auto& arg = (*osc_message)[i];
            juce::ValueTree arg_state("OscArg");

            if (arg.isInt32())
            {
                arg_state.setProperty("type", "int32", nullptr);
                arg_state.setProperty("value", arg.getInt32(), nullptr);
            }
            else if (arg.isFloat32())
            {
                arg_state.setProperty("type", "float32", nullptr);
                arg_state.setProperty("value", arg.getFloat32(), nullptr);
            }
            else if (arg.isString())
            {
                arg_state.setProperty("type", "string", nullptr);
                arg_state.setProperty("value", arg.getString(), nullptr);
            }

            osc_state.appendChild(arg_state, nullptr);
        }

        state.appendChild(osc_state, nullptr);
    }

    return state;
}

void Event::from_value_tree(const juce::ValueTree& state)
{
    if (!state.isValid()) return;

    const juce::String ip = state.getProperty("ip", "");
    ip_address = ip.isNotEmpty() ? std::make_optional(juce::IPAddress(ip)) : std::nullopt;

    const auto osc_state = state.getChildWithName("OscMessage");
    if (osc_state.isValid())
    {
        juce::OSCMessage message(osc_state.getProperty("address").toString());

        for (int i = 0; i < osc_state.getNumChildren(); ++i)
        {
            const auto arg_state = osc_state.getChild(i);
            const juce::String type = arg_state.getProperty("type", "");

            if (type == "int32") message.addInt32(static_cast<int32_t>(arg_state.getProperty("value")));
            else if (type == "float32") message.addFloat32(static_cast<float>(arg_state.getProperty("value")));
            else if (type == "string") message.addString(arg_state.getProperty("value").toString());
        }

        osc_message = std::make_optional(std::move(message));
    }
    else
    {
        osc_message = std::nullopt;
    }
}