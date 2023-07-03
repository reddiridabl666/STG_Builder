cmake_minimum_required(VERSION 3.25)

project(stg_builder_engine)

set(LIBS stg_builder_core stg_builder_ui ImGui-SFML::ImGui-SFML)
add_executable(${PROJECT_NAME} engine/main.cpp engine/app/App.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC engine/app)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
