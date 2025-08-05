cmake_minimum_required(VERSION 3.28)
project(Cypher)

#set(CMAKE_SYSTEM_NAME Windows)

#set(CMAKE_CROSSCOMPILING true)

#set(CMAKE_SYSTEM_PROCESSOR x86_64)

include(FetchContent)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


FetchContent_Declare(
    dirent
    GIT_REPOSITORY https://github.com/tronkko/dirent.git
    GIT_TAG master
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master 
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git 
    GIT_TAG master
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git 
    GIT_TAG master
)


FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)


set(
    SOURCE_FILES 
    ${SRC_DIR}/Graphics/Camera.cpp
    ${SRC_DIR}/Graphics/GLFWCanvas.cpp
    ${SRC_DIR}/Graphics/Shader.cpp  
    ${SRC_DIR}/Graphics/FileReader.cpp
    ${SRC_DIR}/Graphics/ResourceManager.cpp 
    ${SRC_DIR}/Graphics/Renderer.cpp
    ${SRC_DIR}/Graphics/GUI.cpp
    ${SRC_DIR}/Graphics/FrameBuffer.cpp
    ${SRC_DIR}/Graphics/GUIComponent.cpp
    ${SRC_DIR}/Graphics/RenderAPI.cpp

    ${SRC_DIR}/Graphics/EntityManager.cpp
    ${SRC_DIR}/Graphics/SystemManager.cpp
    ${SRC_DIR}/Graphics/ModelLoader.cpp
    ${SRC_DIR}/Core/index.cpp
    ${SRC_DIR}/Core/Application.cpp
)

set (
    GLAD_SOURCE_FILES
    ${EXTERNAL_DIR}/GLAD/glad.c
)

set(
    STB_IMAGE_SOURCE_FILES
    ${EXTERNAL_DIR}/STB_IMAGE/stb_image.cpp
)
      
set(
    IMGUI_SOURCE_FILES
    ${EXTERNAL_DIR}/imgui/imconfig.h
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui.h 
    ${EXTERNAL_DIR}/imgui/imgui_internal.h
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/imstb_rectpack.h
    ${EXTERNAL_DIR}/imgui/imstb_textedit.h
    ${EXTERNAL_DIR}/imgui/imstb_truetype.h
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.h
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.h
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3_loader.h 
    ${EXTERNAL_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
    ${EXTERNAL_DIR}/imgui/misc/cpp/imgui_stdlib.h
)

FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(assimp)


add_executable(
    Cypher
    ${SOURCE_FILES}
    ${GLAD_SOURCE_FILES}
    ${STB_IMAGE_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
)

target_link_libraries(Cypher assimp)
target_link_libraries(Cypher glfw)
target_link_libraries(Cypher glm)
