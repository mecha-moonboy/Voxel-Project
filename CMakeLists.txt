cmake_minimum_required(VERSION 3.10)
include(FetchContent)

project(KybusEngine)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_executable(${PROJECT_NAME} main.cpp Shader.cpp Mesh.cpp)

# SDL2
set(SDL2_DIR "C:/SDL2")
find_library(SDL2_LIBRARY NAMES SDL2 PATHS "${SDL2_DIR}/lib/x86")
find_library(SDL2MAIN_LIBRARY NAMES SDL2main PATHS "${SDL2_DIR}/lib/x86")
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL2_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})

# GLEW
set(GLEW_DIR "C:/GLEW")
find_library(GLEW_LIBRARY NAMES glew32 PATHS "${GLEW_DIR}/lib/Release/Win32")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLEW_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARY})

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# Header only GL Mathematics library
include_directories("GLM")

# Jolt Physics
add_subdirectory(JoltPhysics/Build)  # Path to JoltPhysics folder
include_directories(${CMAKE_SOURCE_DIR}/JoltPhysics)
target_link_libraries(${PROJECT_NAME} PRIVATE Jolt)

# Windows-specific (shell32.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE shell32)

# Copy DLLs after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL2_DIR}/lib/x86/SDL2.dll"
    "${GLEW_DIR}/bin/Release/Win32/glew32.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)