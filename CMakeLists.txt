cmake_minimum_required(VERSION 3.15)

project(EditorCollab)

add_library(EditorCollab SHARED
    main.cpp
)

target_link_libraries(EditorCollab
    Geode
    WebSocket
    nlohmann_json::nlohmann_json
)
