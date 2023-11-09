cmake_minimum_required(VERSION 3.25)

project(stg_builder_core)

set(SOURCES
    window/Window.cpp
    game/collisions/hitbox/Hitbox.cpp
    game/collisions/hitbox/HitboxProps.cpp
    game/collisions/hitbox/HitboxFactory.cpp
    game/mechanics/Movement.cpp
    game/mechanics/Life.cpp
    game/mechanics/actions/ActionFactory.cpp
    game/mechanics/actions/Decorators.cpp
    game/mechanics/Value.cpp
    game/objects/GameField.cpp
    game/objects/Player.cpp
    game/objects/GameObject.cpp
)

set(INCLUDE_DIRS
    window
    events
    base
    assets
    game/objects
    game/ui
    game/mechanics
    game/mechanics/actions
    game/collisions
    game/collisions/hitbox
    game/collisions/rtree
)

set(LIBS
    stg_builder_common
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
