# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

# Declare a new project named 'example_1'. This sets various variables
# related to the project and provides a context for the following CMake
# commands.
project(assignment)

# Search for the OpenGL package on the system. The REQUIRED argument
# ensures that if OpenGL is not found, CMake will produce an error and
# stop processing. If found, this command sets up various variables
# and imported targets related to OpenGL that can be used later.
find_package(OpenGL REQUIRED)

# Add an executable target named 'example_1' that is built from the source
# file 'main.cpp'. This means that CMake will generate build rules to compile
# 'main.cpp' and link it into an executable program named 'example_1'.
add_executable(${PROJECT_NAME} main.cpp
        ChessBoard.h
        ChessBoard.cpp
        ChessPieces.cpp
        ChessPieces.h
        constants.h
)

# Specify libraries to link with the 'example_1' target.
# This tells CMake that when it's linking 'example_1', it should also
# link the following libraries:
# - glfw: This is a windowing library commonly used with OpenGL.
# - glad: A library to load OpenGL extensions.
# - OpenGL::GL: This is an imported target for the main OpenGL library
#               provided by the find_package(OpenGL) command.
target_link_libraries(${PROJECT_NAME} glm glfw glad OpenGL::GL stb framework)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)

#target_compile_definitions(${PROJECT_NAME} PRIVATE
#        RESOURCES_DIR="./resources/")
target_compile_definitions(${PROJECT_NAME} PRIVATE
        RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/")