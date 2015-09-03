cmake_minimum_required(VERSION 2.8)
project(SUscCGL )
set(CMAKE_BUILD_TYPE "Release")

#Include de GLM
include(ExternalProject)
ExternalProject_Add(
  glm
  PREFIX "./library/glm"
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_INSTALL ON
)

#Création des headers des shaders
add_executable(embedfile sources/embedfile.cpp)
execute_process(COMMAND embedfile simple_vert_code shaders/shader.vert include/simple_shader_vert.h)
execute_process(COMMAND embedfile simple_frag_code shaders/shader.frag include/simple_shader_frag.h)
execute_process(COMMAND embedfile smooth_vert_code shaders/smooth_shader.vert include/smooth_shader_vert.h)
execute_process(COMMAND embedfile smooth_frag_code shaders/smooth_shader.frag include/smooth_shader_frag.h)

#Flags and libraries
set (CMAKE_CXX_FLAGS "-std=c++0x -g3 -Wno-deprecated -O0")
find_package(GLUT)
find_package(OpenGL)
find_package(X11)
find_package(GLEW)
set(CORELIBS ${GLEW_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES})

#Include files
include_directories("include/" "library/libmesh/" "${CMAKE_CURRENT_BINARY_DIR}/library/glm/src/glm")
file(GLOB_RECURSE source_files sources/* include/cgl/*)

#Link
add_library(SUscCGL SHARED ${source_files})
add_library(mesh SHARED library/libmesh/libmesh5.h library/libmesh/libmesh5.c)
add_dependencies(SUscCGL glm)
target_link_libraries(SUscCGL ${CORELIBS} mesh)

#Compilation
project(SUcgl)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$ENV{HOME}/bin")
add_executable(SUcgl main.cpp)
target_link_libraries(SUcgl SUscCGL)

#file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
#file(COPY shaders/ DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")



