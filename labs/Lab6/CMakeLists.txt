# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab6 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/main.cpp)

# Add the executable
add_executable(Lab6 ${SOURCES})
target_include_directories(Lab6 PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

# Find and configure OpenMPI
find_package(OpenMPI REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)
link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab2 PUBLIC sfml-graphics sfml-system sfml-window MVAPCH2::MPICXX)