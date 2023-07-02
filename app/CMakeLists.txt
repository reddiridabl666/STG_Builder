cmake_minimum_required(VERSION 3.25)

project(stg_builder_engine)

add_executable(${PROJECT_NAME} engine/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE stg_builder_core)
