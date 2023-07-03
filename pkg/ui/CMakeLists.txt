cmake_minimum_required(VERSION 3.25)

project(stg_builder_ui)

set(SOURCES test.cpp)
set(INCLUDE_DIRS .)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE stg_builder_core ImGui-SFML::ImGui-SFML)
