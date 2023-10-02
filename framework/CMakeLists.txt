cmake_minimum_required(VERSION 3.12)

project(framework)

add_library(framework
        src/Shader.cpp
        src/window.cpp
        include/framework/geometry.h
        include/framework/VertexArrayObject.h
        include/framework/Shader.h
        include/framework/window.h
        src/geometry.cpp
        include/framework/Texture.h
        src/Texture.cpp
        include/framework/Camera.h
        src/Camera.cpp)
target_include_directories(framework PUBLIC include)

target_link_libraries(framework PUBLIC glad glfw glm stb)

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)