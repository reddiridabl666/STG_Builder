cmake_minimum_required(VERSION 3.25)

project(stg_builder_ui)

set(SOURCES
    elements/GameInfo.cpp
    elements/Button.cpp
    elements/Menu.cpp
    elements/LevelInfo.cpp
    elements/Box.cpp
    elements/tabs/MainTab.cpp
    elements/tabs/LevelTab.cpp
    common/ImguiUtils.cpp
)

set(INCLUDE_DIRS
    ..
    .
    common
    mixin
    elements
)

set(LIBS
    stg_builder_common
    stg_builder_core
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE elements)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
