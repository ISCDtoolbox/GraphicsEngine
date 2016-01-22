cmake_minimum_required(VERSION 2.8)
project(SUscCGL)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories("~/include/" "include/" ${PNG_INCLUDE_DIR})

configure_file("${CMAKE_SOURCE_DIR}/include/ext.h.in" "${CMAKE_SOURCE_DIR}/include/ext.h")

#Libraries
find_library(common NAMES Commons HINTS "$ENV{HOME}/lib")
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
if(!APPLE)
	find_library(glew   NAMES GLEW    HINTS "$ENV{HOME}/lib")
	set(CORELIBS ${glew})
endif()
set(CORELIBS ${common} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES})

#Flags and libraries
set(CMAKE_CXX_FLAGS "-std=c++0x -g3 -O2 -w")


#Project files
file(GLOB_RECURSE source_files sources/* include/cgl/* include/*)
add_library(SUscCGL SHARED ${source_files})
target_link_libraries(SUscCGL ${CORELIBS})
add_dependencies(SUscCGL HEADERFILES)

install(TARGETS SUscCGL LIBRARY DESTINATION "$ENV{HOME}/lib/")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "$ENV{HOME}/include/SUscCGL")


#Creates header files from shaders
add_custom_target(
  HEADERFILES
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" simple_frag_code "../shaders/shader.frag"        simple_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" simple_vert_code "../shaders/shader.vert"        simple_shader_vert.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" smooth_frag_code "../shaders/smooth_shader.frag" smooth_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" smooth_vert_code "../shaders/smooth_shader.vert" smooth_shader_vert.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" flat_frag_code   "../shaders/flat_shader.frag"   flat_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" flat_vert_code   "../shaders/flat_shader.vert"   flat_shader_vert.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" fresnel_frag_code   "../shaders/fresnel_shader.frag"   fresnel_shader_frag.h
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" fresnel_vert_code   "../shaders/fresnel_shader.vert"   fresnel_shader_vert.h
  DEPENDS embedfile
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/include"
  COMMENT "Creates header files"
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
add_executable(embedfile sources/embedfile.cpp)
