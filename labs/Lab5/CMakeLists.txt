# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab5 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/SocketServer.cpp
                  ${PROJECT_SOURCE_DIR}/code/ServerTCP.cpp)

# Add the executable
add_executable(SocketServer ${SOURCES})
target_include_directories(SocketServer PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/SocketClient.cpp
                  ${PROJECT_SOURCE_DIR}/code/ClientTCP.cpp)

# Add the executable
add_executable(SocketClient ${SOURCES})
target_include_directories(SocketClient PUBLIC ${PROJECT_SOURCE_DIR}/code/include)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(SocketServer PUBLIC sfml-network sfml-system)
target_link_libraries(SocketClient PUBLIC sfml-network sfml-system)