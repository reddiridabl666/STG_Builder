cmake_minimum_required(VERSION 3.25)

project(stg_builder_core)

set(SOURCES
    window/Window.cpp
    game/collisions/hitbox/Hitbox.cpp
    game/mechanics/Movement.cpp
    game/objects/GameInfo.cpp
    game/objects/GameField.cpp
)

set(INCLUDE_DIRS
    window
    events
    base
    game/objects
    game/mechanics
    game/collisions
    game/collisions/hitbox
    game/collisions/rtree
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
