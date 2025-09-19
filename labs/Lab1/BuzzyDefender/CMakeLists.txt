# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab1 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/BuzzyDefender.cpp
                  ${PROJECT_SOURCE_DIR}/code/ECE_Buzzy.cpp)

# Add the executable
add_executable(BuzzyDefender ${SOURCES})
target_include_directories(BuzzyDefender PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(BuzzyDefender PUBLIC sfml-graphics sfml-system sfml-window)

