cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(ULC)

# add -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# add -fPIC to CXX flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

SET(CMAKE_EXE_LINKER_FLAGS 
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${CMAKE_SOURCE_DIR}")
# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:nlohmann/json@3.10.5")
CPMAddPackage("gh:gabime/spdlog@1.12.0")
CPMAddPackage("gh:CrowCpp/Crow@1.0+5")
CPMAddPackage("gh:daniele77/cli@2.1.0")

# add include/ directory
include_directories(include)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)

# add all files under src/common as common.a
file(GLOB_RECURSE COMMON_SOURCES src/common/*.cpp)
add_library(common STATIC ${COMMON_SOURCES})
target_link_libraries(common pthread nlohmann_json::nlohmann_json spdlog::spdlog Crow::Crow cli::cli)
# link common.a to all executables
link_libraries(common)

# add dynamic library testplugin.so
add_library(testplugin SHARED src/plugins/testplugin/testplugin.cpp)
target_link_libraries(testplugin common)

file(GLOB_RECURSE SERVER_SOURCES src/server/*.cpp)
# add executable
add_executable(server ${SERVER_SOURCES})

file(GLOB_RECURSE CLIENT_SOURCES src/client/*.cpp)
# add executable
add_executable(client ${CLIENT_SOURCES})

add_executable(genconfig src/genconfig.cpp)