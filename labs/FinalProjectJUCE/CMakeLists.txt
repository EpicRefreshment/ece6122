# Minimum CMake version required
cmake_minimum_required(VERSION 3.22)

# Project name and version
project(MultimodeSequencer VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add JUCE as a subdirectory.
# Replace <path_to_juce> with the actual path to your JUCE repository.
add_subdirectory(../../../../JUCE) # Assumes JUCE is in a parallel directory

add_executable(MultimodeSequencer
    code/main.cpp
)

target_include_directories(MultimodeSequencer PUBLIC
    code/include
)

# Link the necessary JUCE modules
target_link_libraries(MultimodeSequencer PUBLIC
    juce_audio_basics
    juce_audio_devices
    juce_audio_formats
    juce_audio_processors
    juce_audio_utils
    juce_core
    juce_data_structures
    juce_events
    juce_graphics
    juce_gui_basics
    juce_gui_extra
)