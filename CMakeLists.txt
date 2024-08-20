cmake_minimum_required(VERSION 3.15)

project(EditorCollab)

# Include the nlohmann_json library
include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(json)

add_library(EditorCollab SHARED
    main.cpp
)

target_link_libraries(EditorCollab
    Geode
    nlohmann_json::nlohmann_json
)
