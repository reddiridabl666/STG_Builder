cmake_minimum_required(VERSION 3.25)

project(stg_builder_gen)

set(SOURCES
    generators/ObjectTypeFactory.cpp
    generators/ObjectOptionsFactory.cpp
    assets/AssetProcessor.cpp
    objects/ObjectOptions.cpp
    objects/ObjectType.cpp
)

set(INCLUDE_DIRS
    .
    assets
    generators
    objects
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE stg_builder_core)
