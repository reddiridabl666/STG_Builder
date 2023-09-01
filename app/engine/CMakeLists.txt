cmake_minimum_required(VERSION 3.25)

project(stg_builder_engine)

set(LIBS
    stg_builder_common
    stg_builder_core
    stg_builder_ui
    stg_builder_gen
)

set(INCLUDE_DIRS
    level
    app
)

set (SOURCES
    app/App.cpp
    app/AppFactory.cpp
    main.cpp
    level/Level.cpp
    level/LevelLoader.cpp
    level/LevelManager.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
