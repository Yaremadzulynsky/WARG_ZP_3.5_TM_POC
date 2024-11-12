# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(TelemetryManager)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the main source directory
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../..")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/TelemetryManager/Inc)
include_directories(${PROJECT_SOURCE_DIR}/TelemetryManager/Tests/Integration)
include_directories(${PROJECT_SOURCE_DIR}/Tools)
include_directories(${PROJECT_SOURCE_DIR}/Boardfiles)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/common/circular_buffer/inc)
include_directories(    ${PROJECT_SOURCE_DIR}/TelemetryManager/Inc
${PROJECT_SOURCE_DIR}/TelemetryManager/Inc/Official_Mavlink_2_Library
${PROJECT_SOURCE_DIR}/TelemetryManager/Inc/Official_Mavlink_2_Library/common
${PROJECT_SOURCE_DIR}/TelemetryManager/Inc/Official_Mavlink_2_Library/message_definitions
${PROJECT_SOURCE_DIR}/TelemetryManager/Inc/Official_Mavlink_2_Library/minimal
${PROJECT_SOURCE_DIR}/TelemetryManager/Inc/Official_Mavlink_2_Library/standard)


# Gather all source files, excluding CMake-generated files and unnecessary files
file(GLOB_RECURSE SOURCES 
    "${PROJECT_SOURCE_DIR}/TelemetryManager/Src/*.cpp"
    "${PROJECT_SOURCE_DIR}/TelemetryManager/Tests/Integration/*.cpp"
    "${PROJECT_SOURCE_DIR}/Drivers/common/circular_buffer/src/*.cpp"
)

# Exclude `GroundStationCommunication.cpp` if not needed and also CMake-specific files
list(FILTER SOURCES EXCLUDE REGEX "/GroundStationCommunication.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*/CMakeFiles/.*CompilerIdCXX.*")

# Add executable for integration test with all sources
add_executable(IntegrationTest ${SOURCES})

# Print debug information to verify paths
message("Project Source Directory: ${PROJECT_SOURCE_DIR}")
message("Sources: ${SOURCES}")
