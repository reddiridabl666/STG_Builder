cmake_minimum_required(VERSION 3.25)

project(stg_builder_gen)

set(SOURCES
    generators/ObjectTypeFactory.cpp
    generators/PatternFactory.cpp
    generators/ObjectOptionsFactory.cpp
    generators/FuncBuilder.cpp
    objects/ObjectOptions.cpp
    objects/ObjectType.cpp
    game/GameState.cpp
    game/PlayerManager.cpp
    level/Level.cpp
    level/LevelLoader.cpp
    level/LevelManager.cpp
    game/SideMenu.cpp
)

set(INCLUDE_DIRS
    .
    assets
    generators
    objects
    game
    level
)

set(LIBS
    stg_builder_core
)

if (DEBUG)
    set(LIBS ${LIBS} stg_builder_ui)
endif()

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
