cmake_minimum_required(VERSION 3.30)
project(PROJEKTINZ)


set(GLFW_INCLUDE_DIR "lib/glfw/include")
set(GLFW_LIB_DIR "lib/glfw/lib-mingw-w64")

set(CMAKE_CXX_STANDARD 20)

include_directories(${GLFW_INCLUDE_DIR})
include_directories(lib/glad/include)
link_directories(${GLFW_LIB_DIR})

# --- GLM (FetchContent) ---------------------
include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git

)
FetchContent_MakeAvailable(glm)

add_executable(PROJEKTINZ main.cpp lib/glad/src/glad.c
        Shader.cpp
        Shader.h
        lib/stb_image.h
        Texture.cpp
        Texture.h
        Camera.cpp
        Camera.h
        Map.cpp
        Map.h
        Enemy.cpp
        Enemy.h
        Gun.cpp
        Gun.h)
target_link_libraries(PROJEKTINZ glfw3.dll glm::glm)