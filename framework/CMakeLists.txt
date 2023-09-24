cmake_minimum_required(VERSION 3.12)

project(framework)

add_library(framework src/Shader.cpp src/window.cpp)
target_include_directories(framework PUBLIC include)

target_link_libraries(framework PUBLIC glad glfw)
