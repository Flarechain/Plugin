cmake_minimum_required(VERSION 3.22)

project(Flarechain VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "@loader_path/../Frameworks")
set(CMAKE_MACOSX_RPATH TRUE)

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

CPMAddPackage(
        NAME catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG v3.14.0
)

if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|ARM64")
        set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.0/onnxruntime-osx-arm64-1.23.0.tgz)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.0/onnxruntime-osx-x86_64-1.23.0.tgz)
    else ()
        message(FATAL_ERROR "Unsupported Mac architecture")
    endif()
elseif(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|ARM64")
        set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.0/onnxruntime-win-arm64-1.23.0.tgz)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.0/onnxruntime-win-x64-1.23.0.tgz)
    else ()
        message(FATAL_ERROR "Unsupported Windows architecture")
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64|arm64|ARM64")
        set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.0/onnxruntime-linux-aarch64-1.23.0.tgz)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.0/onnxruntime-linux-x64-1.23.0.tgz)
    else ()
        message(FATAL_ERROR "Unsupported Linux architecture")
    endif()
endif()

CPMAddPackage(
        NAME onnxruntime
        URL ${ONNX_URL}
)

add_library(onnxruntime SHARED IMPORTED GLOBAL)

if(APPLE)
    set(ORT_LIB ${onnxruntime_SOURCE_DIR}/lib/libonnxruntime.dylib)
elseif(WIN32)
    set(ORT_LIB ${onnxruntime_SOURCE_DIR}/lib/onnxruntime.lib)
elseif(UNIX)
    set(ORT_LIB ${onnxruntime_SOURCE_DIR}/lib/libonnxruntime.so)
endif()

set_target_properties(onnxruntime PROPERTIES
        IMPORTED_LOCATION ${ORT_LIB}
        INTERFACE_INCLUDE_DIRECTORIES ${onnxruntime_SOURCE_DIR}/include
)

file(GLOB PluginCoreFiles src/plugin_core/*)
file(GLOB CoreFiles src/core/*)
file(GLOB ModelFiles src/model/*)
file(GLOB_RECURSE GuiComponentsFiles src/gui_components/*)
list(FILTER GuiComponentsFiles EXCLUDE REGEX ".*\\.DS_Store$")
set(SourceFiles
        ${ModelFiles}
        ${PluginCoreFiles}
        ${CoreFiles}
        ${GuiComponentsFiles}
        ${TestFiles}
)

set(PLUGIN_FORMATS VST3 AU Standalone)
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
        FORMATS ${PLUGIN_FORMATS}
)

foreach(format ${PLUGIN_FORMATS})
    if(TARGET ${PROJECT_NAME}_${format})
        if(APPLE)
            add_custom_command(TARGET ${PROJECT_NAME}_${format} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}_${format}>/Frameworks"

                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${ORT_LIB}
                    "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}_${format}>/Frameworks/libonnxruntime.1.23.0.dylib"

                    COMMENT "Copying onnxruntime into ${format} bundle"
            )
            set_target_properties(${PROJECT_NAME}_${format} PROPERTIES
                    SKIP_BUILD_RPATH FALSE
                    BUILD_WITH_INSTALL_RPATH TRUE
                    INSTALL_RPATH "@loader_path/../Frameworks"
            )
        elseif(WIN32)
            add_custom_command(TARGET ${PROJECT_NAME}_${format} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${onnxruntime_SOURCE_DIR}/lib/onnxruntime.dll
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}_${format}>"

                    COMMENT "Copying onnxruntime dll for ${format}"
            )
        elseif(UNIX)
            add_custom_command(TARGET ${PROJECT_NAME}_${format} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${ORT_LIB}
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}_${format}>"

                    COMMAND patchelf --set-rpath "$$ORIGIN"
                    "$<TARGET_FILE:${PROJECT_NAME}_${format}>"

                    COMMENT "Copying onnxruntime so for ${format}"
            )
        endif()
    endif()
endforeach()

foreach(format ${PLUGIN_FORMATS})
    if(TARGET ${PROJECT_NAME}_${format})
        if(APPLE)
            add_custom_command(TARGET ${PROJECT_NAME}_${format} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}_${format}>/Resources/Models"

                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/ai_models/default.onnx"
                    "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}_${format}>/Resources/Models/default.onnx"

                    COMMENT "Copying ONNX model into ${format} bundle"
            )

        elseif(WIN32)
            add_custom_command(TARGET ${PROJECT_NAME}_${format} POST_BUILD

                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}_${format}>/Resources/Models"

                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/ai_models/default.onnx"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}_${format}>/Resources/Models/default.onnx"

                    COMMENT "Copying ONNX model for ${format}"
            )

        elseif(UNIX)
            add_custom_command(TARGET ${PROJECT_NAME}_${format} POST_BUILD

                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}_${format}>/Resources/Models"

                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/ai_models/default.onnx"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}_${format}>/Resources/Models/default.onnx"

                    COMMENT "Copying ONNX model for ${format}"
            )
        endif()

    endif()
endforeach()

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
        onnxruntime
)

file(GLOB_RECURSE TestFiles src/tests/*)
list(FILTER TestFiles EXCLUDE REGEX ".*\\.DS_Store$")

add_executable(tests
        ${TestFiles}
        ${ModelFiles}
        ${CoreFiles}
)
target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain
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
        onnxruntime
)