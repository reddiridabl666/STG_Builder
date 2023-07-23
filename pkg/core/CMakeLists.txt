cmake_minimum_required(VERSION 3.25)

project(stg_builder_core)

set(SOURCES window/WindowSFML.cpp game/collisions/hitbox/Hitbox.cpp)
set(INCLUDE_DIRS
    window
    events
    base
    game
    game/collisions
    game/collisions/hitbox
    game/collisions/rtree
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
