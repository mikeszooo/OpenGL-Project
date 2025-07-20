cmake_minimum_required(VERSION 3.30)
project(PROJEKTINZ)

set(GLFW_INCLUDE_DIR "lib/glfw/include")
set(GLFW_LIB_DIR "lib/glfw/lib-mingw-w64")

set(CMAKE_CXX_STANDARD 20)

include_directories(${GLFW_INCLUDE_DIR})
include_directories(lib/glad/include)
include_directories(lib)  # dla stb_image.h

link_directories(${GLFW_LIB_DIR})

# --- GLM (FetchContent) ---------------------
include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

add_executable(PROJEKTINZ
        src/main.cpp
        lib/glad/src/glad.c
        src/Shader.cpp
        src/Shader.h
        src/Texture.cpp
        src/Texture.h
        src/Camera.cpp
        src/Camera.h
        src/Map.cpp
        src/Map.h
        src/Enemy.cpp
        src/Enemy.h
        src/Gun.cpp
        src/Gun.h
)

target_link_libraries(PROJEKTINZ glfw3.dll glm::glm)
