cmake_minimum_required(VERSION 3.25)

project(stg_builder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(SFML_VERSION 2.5.1)
set(IMGUI_VERSION 1.89.6)
set(IMGUI_SFML_VERSION 2.5)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML
  GIT_TAG        master
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG        master
)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG        master
)

# SFML
option(SFML_BUILD_AUDIO "Build audio" ON)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

# Dear ImGui
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

FetchContent_MakeAvailable(imgui-sfml)

set(ERROR_FLAGS -Werror -Wall -Wpedantic)

# Add target to build std modules
#set(SYSTEM_MODULES iostream string)
#add_custom_target(std_modules ALL
#    COMMAND ${CMAKE_COMMAND} -E echo "Building standard library modules"
#    COMMAND g++ -fmodules-ts -std=c++23 -c -x c++-system-header ${SYSTEM_MODULES}
#    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#)

# Function to set up modules in GCC
function (prepare_for_module TGT)
    target_compile_options(${TGT} PUBLIC -fmodules-ts -x c++)
    set_property(TARGET ${TGT} PROPERTY CXX_STANDARD 23)
    set_property(TARGET ${TGT} PROPERTY CXX_EXTENSIONS OFF)
#    add_dependencies(${TGT} std_modules)
endfunction()

# Program name and sources
set (TARGET main)
set (SOURCES src/app/main.cpp)
set (MODULES src/pkg/window.cppm)

# Setup program modules object library
set (MODULE_TARGET prog-modules)
add_library(${MODULE_TARGET} OBJECT ${MODULES})
target_link_libraries(${MODULE_TARGET} PRIVATE ImGui-SFML::ImGui-SFML)
prepare_for_module(${MODULE_TARGET})

# Setup executable
add_executable(${TARGET} ${SOURCES})
prepare_for_module(${TARGET})

# Add modules to application using object library
target_link_libraries(${TARGET} PRIVATE ${MODULE_TARGET})

target_compile_options(${TARGET} PRIVATE ${ERROR_FLAGS})

option(SANITIZE "Sanitizer flags" OFF)
option(DEBUG "Add debug info" OFF)

if(DEBUG)
  target_compile_options(${TARGET} PRIVATE -g)
endif()

if(SANITIZE)
  set(SANITIZER_FLAGS -fsanitize=address,leak,undefined)
  target_compile_options(${TARGET} PRIVATE ${SANITIZER_FLAGS})
  target_link_options(${TARGET} PRIVATE ${SANITIZER_FLAGS})
endif()
