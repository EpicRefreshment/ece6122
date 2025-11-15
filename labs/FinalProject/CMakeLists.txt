# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MultimodeSequencer VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/main.cpp
                  ${PROJECT_SOURCE_DIR}/code/SequencerEngine.cpp
                  ${PROJECT_SOURCE_DIR}/code/SeqTrack.cpp
                  ${PROJECT_SOURCE_DIR}/code/GlobalControlPanel.cpp
                  ${PROJECT_SOURCE_DIR}/code/StepGrid.cpp
                  ${PROJECT_SOURCE_DIR}/code/SampleControlPanel.cpp
                  ${PROJECT_SOURCE_DIR}/code/TrackControlPanel.cpp
                  ${PROJECT_SOURCE_DIR}/code/ThreadPool.cpp)

# Add the executable
add_executable(MultimodeSequencer ${SOURCES})
target_include_directories(MultimodeSequencer PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

# Find and configure OpenMP
find_package(OpenMP REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(MultimodeSequencer PUBLIC sfml-graphics sfml-system sfml-window sfml-audio OpenMP::OpenMP_CXX)

file(COPY "${PROJECT_SOURCE_DIR}/fonts" DESTINATION "${COMMON_OUTPUT_DIR}/bin/")
file(COPY "${PROJECT_SOURCE_DIR}/samples" DESTINATION "${COMMON_OUTPUT_DIR}/bin/")