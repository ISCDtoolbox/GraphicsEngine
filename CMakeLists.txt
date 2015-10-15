cmake_minimum_required(VERSION 2.8)
project(SUscCGL )
set(CMAKE_BUILD_TYPE "Release")


set(CMAKE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})
configure_file("${CMAKE_SOURCE_DIR}/include/ext.h.in" "${CMAKE_SOURCE_DIR}/include/ext.h")

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


#Flags and libraries
set(CMAKE_CXX_FLAGS "-std=c++0x -g3 -Wno-deprecated -O0")
find_package(GLUT)
find_package(OpenGL)
find_package(X11)
find_package(GLEW)


set(CORELIBS ${GLEW_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES} )

#Include files
include_directories("include/" "library/libmesh/" ${PNG_INCLUDE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/library/glm/src/glm/")
file(GLOB_RECURSE source_files sources/* include/cgl/* include/*)

#Link
add_library(SUscCGL SHARED ${source_files})
add_dependencies(SUscCGL glm)
add_library(mesh SHARED library/libmesh/libmesh5.h library/libmesh/libmesh5.c)
target_link_libraries(SUscCGL ${CORELIBS} mesh)
add_dependencies(SUscCGL HEADERFILES)

#Installation lors d'un make install, permet de ne builder que la librairie.
set(IN /usr)
#set(IN $ENV{HOME})
#set(IN /home/foulonj)

install(TARGETS mesh
        ARCHIVE DESTINATION "${IN}/lib/"
        LIBRARY DESTINATION "${IN}/lib/"
        RUNTIME DESTINATION "${IN}/bin/")

install(TARGETS SUscCGL
        ARCHIVE DESTINATION "${IN}/lib/"
        LIBRARY DESTINATION "${IN}/lib/"
        RUNTIME DESTINATION "${IN}/bin/")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/"
        DESTINATION "${IN}/include/SUscCGL")

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/library/glm/src/glm/glm/"
	DESTINATION "${IN}/include/glm")


add_custom_target(
  HEADERFILES
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" simple_frag_code "../shaders/shader.frag"        simple_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" simple_vert_code "../shaders/shader.vert"        simple_shader_vert.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" smooth_frag_code "../shaders/smooth_shader.frag" smooth_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" smooth_vert_code "../shaders/smooth_shader.vert" smooth_shader_vert.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" flat_frag_code   "../shaders/flat_shader.frag"   flat_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" flat_vert_code   "../shaders/flat_shader.vert"   flat_shader_vert.h
  DEPENDS embedfile
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/include"
  COMMENT "Creates header files"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
add_executable(embedfile sources/embedfile.cpp)







