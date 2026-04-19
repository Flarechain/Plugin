#include <catch2/catch_test_macros.hpp>
#include "../core/OscParser.h"

TEST_CASE("Fails with invalid OSC address", "[osc][parser]")
{
    REQUIRE(OscParser::parse("") == std::nullopt);
    REQUIRE(OscParser::parse(" ") == std::nullopt);
    REQUIRE(OscParser::parse("synth") == std::nullopt);
    REQUIRE(OscParser::parse("synth ") == std::nullopt);
    REQUIRE(OscParser::parse("synth 1") == std::nullopt);
    REQUIRE(OscParser::parse("synth/message") == std::nullopt);
    REQUIRE(OscParser::parse("synth/message 1") == std::nullopt);
}

TEST_CASE("Parses valid OSC address, no arguments", "[osc][parser]")
{
    auto osc_message = OscParser::parse("/synth").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 0));

    osc_message = OscParser::parse("/synth/message").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth/message" &&
        osc_message.size() == 0));

    osc_message = OscParser::parse("/synth/message ").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth/message" &&
        osc_message.size() == 0));
}

TEST_CASE("Parses valid OSC address, one bool argument", "[osc][parser]")
{
    auto osc_message = OscParser::parse("/synth true").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == 1));

    osc_message = OscParser::parse("/synth TRUE").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == 1));

    osc_message = OscParser::parse("/synth false").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == 0));

    osc_message = OscParser::parse("/synth FALSE").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == 0));
}

TEST_CASE("Parses valid OSC address, one int argument", "[osc][parser]")
{
    auto osc_message = OscParser::parse("/synth 1").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == 1));

    osc_message = OscParser::parse("/synth -1").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == -1));

    osc_message = OscParser::parse("/synth 0").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isInt32() &&
        osc_message[0].getInt32() == 0));
}

TEST_CASE("Parses valid OSC address, one float argument", "[osc][parser]")
{
    auto osc_message = OscParser::parse("/synth 1.5").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isFloat32() &&
        osc_message[0].getFloat32() == 1.5f));

    osc_message = OscParser::parse("/synth -0.5").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isFloat32() &&
        osc_message[0].getFloat32() == -0.5f));

    osc_message = OscParser::parse("/synth 0.0").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isFloat32() &&
        osc_message[0].getFloat32() == 0.0f));

    osc_message = OscParser::parse("/synth -0.0").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isFloat32() &&
        osc_message[0].getFloat32() == -0.0f));
}

TEST_CASE("Parses valid OSC address, one string argument", "[osc][parser]")
{
    auto osc_message = OscParser::parse("/synth play").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isString() &&
        osc_message[0].getString() == "play"));

    osc_message = OscParser::parse("/synth \"turn on\"").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isString() &&
        osc_message[0].getString() == "turn on"));

    osc_message = OscParser::parse("/synth -0.5-").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isString() &&
        osc_message[0].getString() == "-0.5-"));

    osc_message = OscParser::parse("/synth 1.").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isString() &&
        osc_message[0].getString() == "1."));

    osc_message = OscParser::parse("/synth .6").value();
    REQUIRE((osc_message.getAddressPattern().toString() == "/synth" &&
        osc_message.size() == 1 &&
        osc_message[0].isString() &&
        osc_message[0].getString() == ".6"));
}