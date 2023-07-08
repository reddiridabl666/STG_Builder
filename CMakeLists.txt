cmake_minimum_required(VERSION 3.25)

project(stg_builder)

add_compile_options(-std=c++2b)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(SFML_VERSION 2.6)
set(IMGUI_VERSION 1.89.6)
set(IMGUI_SFML_VERSION 2.5)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG master
)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG master
)

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG master
)

# SFML
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

# Dear ImGui
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

FetchContent_MakeAvailable(imgui-sfml)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

option(SANITIZE "Sanitizer flags" OFF)
option(DEBUG "Add debug info" OFF)

if (DEBUG)
    target_compile_options(${TARGET} PRIVATE -g)
endif ()

if (SANITIZE)
    set(SANITIZER_FLAGS -fsanitize=address,leak,undefined)
    add_compile_options(${SANITIZER_FLAGS})
    add_link_options(${SANITIZER_FLAGS})
endif ()

find_package(Boost 1.80.0) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

add_subdirectory(pkg/core)
add_subdirectory(pkg/ui)
add_subdirectory(app)
