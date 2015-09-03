cmake_minimum_required(VERSION 2.8)

project(GLIC)

set(CMAKE_BUILD_TYPE "Release")

#Include de GLM
include(ExternalProject)
ExternalProject_Add(
  glm
  PREFIX "./glm"
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_INSTALL ON
)

#Move shaders to build directory
#configure_file(data_file ${CMAKE_CURRENT_BINARY_DIR}/data_file COPYONLY)
#file(COPY shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


#Options C++ (-Wno-deprecated: remove glut deprecated)

set (CMAKE_CXX_FLAGS "-std=c++0x -g3 -Wno-deprecated -O0")
# -stdlib=libc++
#Options graphiques
find_package(GLUT)
find_package(OpenGL)
find_package(X11)
find_package(GLEW)

set(CORELIBS ${GLEW_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES})

#Include files
#include_directories("include/")
include_directories("include/" "libmesh/" "${CMAKE_CURRENT_BINARY_DIR}/glm/src/glm")

#Compilation
#Recuperation de la liste des fichiers .cpp et .h
file(GLOB_RECURSE source_files sources/* include/glic/* )
add_library(GLIC SHARED ${source_files})
add_library(mesh SHARED libmesh/libmesh5.h libmesh/libmesh5.c)
add_dependencies(GLIC glm)
target_link_libraries(GLIC ${CORELIBS} mesh)

project(main)
add_executable(main main.cpp)
target_link_libraries(main GLIC)
