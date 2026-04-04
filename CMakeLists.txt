cmake_minimum_required(VERSION 3.10)

project(TradingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(include)

# Collect all source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Main executable
add_executable(Trading_Engine ${SOURCES})