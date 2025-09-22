# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(BuzzyDefender VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/main.cpp
                  ${PROJECT_SOURCE_DIR}/code/ECE_Defender.cpp
                  ${PROJECT_SOURCE_DIR}/code/ECE_Buzzy.cpp
                  ${PROJECT_SOURCE_DIR}/code/ECE_Enemy.cpp
                  ${PROJECT_SOURCE_DIR}/code/ECE_LaserBlast.cpp)

# Add the executable
add_executable(Lab1 ${SOURCES})
target_include_directories(Lab1 PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab1 PUBLIC sfml-graphics sfml-system sfml-window)

file(COPY "${PROJECT_SOURCE_DIR}/graphics" DESTINATION "${COMMON_OUTPUT_DIR}/bin/")

