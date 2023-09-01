cmake_minimum_required(VERSION 3.25)

project(stg_builder_common)

set(SOURCES
    Messages.cpp
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
