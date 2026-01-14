cmake_minimum_required(VERSION 3.28)  
#set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(Cypher)

#set(CMAKE_SYSTEM_NAME Windows)

#set(CMAKE_CROSSCOMPILING true)

#set(CMAKE_SYSTEM_PROCESSOR x86_64)

include(FetchContent)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL 3 REQUIRED)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)


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
     
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(imgui)


add_executable(
    Cypher
    ${SOURCE_FILES}
    ${GLAD_SOURCE_FILES}
    ${STB_IMAGE_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES} 
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(Cypher PRIVATE nlohmann_json::nlohmann_json) 
target_link_libraries(Cypher PRIVATE assimp)
target_link_libraries(Cypher PRIVATE glfw)
target_link_libraries(Cypher PRIVATE glm)

target_include_directories(Cypher
PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/imgui
  ${CMAKE_CURRENT_LIST_DIR}/imgui/backends
)
#target_link_libraries(Cypher PRIVATE imgui)

