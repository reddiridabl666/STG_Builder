cmake_minimum_required(VERSION 3.25)

project(stg_builder)

set(SOURCES
    main.cpp
    app/App.cpp
)

set(INCLUDE_DIRS
    app
)

set(LIBS
    stg_builder_common
    stg_builder_ui
    stg_builder_core
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
