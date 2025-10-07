# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CellularAutomata VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/main.cpp
                  ${PROJECT_SOURCE_DIR}/code/Cell.cpp
                  ${PROJECT_SOURCE_DIR}/code/Cell_Window.cpp
                  ${PROJECT_SOURCE_DIR}/code/Command_Parser.cpp)

# Add the executable
add_executable(Lab2 ${SOURCES})
target_include_directories(Lab2 PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

# Find and configure OpenMP
find_package(OpenMP REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab2 PUBLIC sfml-graphics sfml-system sfml-window OpenMP::OpenMP_CXX)

