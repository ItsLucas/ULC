cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(ULC)

# add executable
add_executable(server src/server/main.cpp)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:nlohmann/json@3.10.5")

# link dependencies
target_link_libraries(server nlohmann_json::nlohmann_json)