cmake_minimum_required(VERSION 3.25)
project(Gengine)

include(FetchContent)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

find_package(glfw3 3.3.8 REQUIRED)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git 
  GIT_TAG master)

set(SOURCES 
  ${SRC_DIR}/Graphics/index.cpp
  ${SRC_DIR}/Graphics/Camera.cpp
  ${SRC_DIR}/Graphics/GLCanvas.cpp
  ${SRC_DIR}/Graphics/Shader.cpp
  ${SRC_DIR}/Graphics/Globals.cpp
  ${SRC_DIR}/Graphics/Model.cpp
  ${SRC_DIR}/Graphics/Mesh.cpp
  ${SRC_DIR}/Graphics/ResourceManager.cpp
  ${SRC_DIR}/Graphics/FileReader.cpp
  ${EXTERNAL_DIR}/GLAD/glad.c
  ${EXTERNAL_DIR}/STB_IMAGE/stb_image.cpp
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
  ${EXTERNAL_DIR}/imgui/imgui_impl_glfw.cpp
  ${EXTERNAL_DIR}/imgui/imgui_impl_glfw.h
  ${EXTERNAL_DIR}/imgui/imgui_impl_opengl3.cpp
  ${EXTERNAL_DIR}/imgui/imgui_impl_opengl3.h
  ${EXTERNAL_DIR}/imgui/imgui_impl_opengl3_loader.h 
  ${EXTERNAL_DIR}/imgui/imgui_stdlib.cpp
  ${EXTERNAL_DIR}/imgui/imgui_stdlib.h

  )


add_executable(Gengine ${SOURCES})

target_link_libraries(Gengine assimp)
target_link_libraries(Gengine glfw)
