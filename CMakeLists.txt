cmake_minimum_required(VERSION 3.22)

project(Flarechain VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CPM_DOWNLOAD_VERSION 0.42.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

# install CPM if it's not installed
if (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.10
)

file(GLOB PluginCoreFiles src/plugin_core/*)
file(GLOB ModelFiles src/model/*)
file(GLOB_RECURSE GuiComponentsFiles src/gui_components/*)
list(FILTER GuiComponentsFiles EXCLUDE REGEX ".*\\.DS_Store$")
set(SourceFiles
        ${ModelFiles}
        ${PluginCoreFiles}
        ${GuiComponentsFiles}
)

juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME "Dennis Battisti"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Deba
        PLUGIN_CODE Flar
        PRODUCT_NAME "Flarechain"
        BUNDLE_ID "com.dennisbattisti.flarechain"
        FORMATS VST3 AU Standalone
)

# Collect all assets files
file(GLOB_RECURSE AssetFiles ${CMAKE_SOURCE_DIR}/assets/*)
list(FILTER AssetFiles EXCLUDE REGEX ".*\\.DS_Store$")
message(AssetFiles: ${AssetFiles})

# Use assets files as embedded binary data
juce_add_binary_data(AssetsData SOURCES ${AssetFiles})
target_link_libraries(${PROJECT_NAME} PRIVATE AssetsData)

# Appearance of source files inside IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFiles})

# Make the Source files buildable
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

# Toggleable options from the JUCE CMake API
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISABLE_COREGRAPHICS_FONT_SMOOTHING=1
)

# Disable shadowing warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shadow)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        juce::juce_analytics
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_osc
)
