cmake_minimum_required(VERSION 3.25)

project(stg_builder_core)

set(SOURCES window/WindowSFML.cpp)
set(INCLUDE_DIRS window events drawable base game)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
