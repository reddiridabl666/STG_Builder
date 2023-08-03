cmake_minimum_required(VERSION 3.25)

project(stg_builder_engine)

set(LIBS
    stg_builder_core
    stg_builder_ui
    stg_builder_gen
    ImGui-SFML::ImGui-SFML
)

set(INCLUDE_DIRS
    engine/level
    engine/app
)

add_executable(${PROJECT_NAME} engine/main.cpp engine/app/App.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
