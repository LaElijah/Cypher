cmake_minimum_required(VERSION 3.25)
project(Gengine)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

find_package(glfw3 3.3.8 REQUIRED)


set(SOURCES 
  ${SRC_DIR}/Graphics/index.cpp
  ${SRC_DIR}/Graphics/Camera.cpp
  ${SRC_DIR}/Graphics/GLCanvas.cpp
  ${SRC_DIR}/Graphics/Shader.cpp
  ${SRC_DIR}/Graphics/Globals.cpp
  ${EXTERNAL_DIR}/GLAD/glad.c
  ${EXTERNAL_DIR}/STB_IMAGE/stb_image.h
  ${EXTERNAL_DIR}/STB_IMAGE/stb_image.cpp
  )



add_executable(Gengine ${SOURCES})


target_link_libraries(Gengine glfw)
