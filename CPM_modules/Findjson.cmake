include("/home/lucas/ULC/cmake/CPM_0.38.6.cmake")
CPMAddPackage("GITHUB_REPOSITORY;nlohmann/json;VERSION;3.10.5;EXCLUDE_FROM_ALL;YES;SYSTEM;YES;")
set(json_FOUND TRUE)