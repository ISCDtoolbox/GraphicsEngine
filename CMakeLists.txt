################################################################
# INITIALIZATION
################################################################
cmake_minimum_required(     VERSION 2.8)
set( CMAKE_BUILD_TYPE       "Release")
set( CMAKE_MACOSX_RPATH     1)
project( GraphicsEngine )
set( CMAKE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})
set( CMAKE_CXX_FLAGS        "-std=c++0x -g3 -O2 -w")
include_directories(        "~/include/" "include/" ${PNG_INCLUDE_DIR})
configure_file(             "${CMAKE_SOURCE_DIR}/include/ext.h.in" "${CMAKE_SOURCE_DIR}/include/ext.h")

################################################################
#GraphicsEngine dependencies
################################################################
find_library( common NAMES Commons HINTS "$ENV{HOME}/lib")
find_package( GLUT REQUIRED)
find_package( OpenGL REQUIRED)
find_package( X11 REQUIRED)
set(CORELIBS  ${common} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES})
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	find_library( glew NAMES GLEW    HINTS "$ENV{HOME}/lib")
	set(CORELIBS  ${common} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES} ${glew})
endif()

################################################################
#GraphicsEngine library
################################################################
file( GLOB_RECURSE     source_files sources/* include/cgl/* include/*)
add_library(           GraphicsEngine SHARED ${source_files})
target_link_libraries( GraphicsEngine ${CORELIBS})
add_dependencies(      GraphicsEngine SHADERFILES)

################################################################
#NumeRO application
################################################################
project(Numero)
set( NUMERO_PATH       "${CMAKE_SOURCE_DIR}/APP_numero/")
configure_file(        "${NUMERO_PATH}/numero.cpp" "${NUMERO_PATH}/numero.cpp")
include_directories(   ~/include/  ~/include/GraphicsEngine/)
add_executable(        numero      "${NUMERO_PATH}/numero.cpp")
add_dependencies(      numero      GraphicsEngine)
target_link_libraries( numero      GraphicsEngine)

################################################################
#Chemistry application
################################################################
project(Chemistry)
set( CHEM_PATH       "${CMAKE_SOURCE_DIR}/APP_chemistry/")
configure_file(        "${CHEM_PATH}/chemistry.cpp" "${CHEM_PATH}/chemistry.cpp")
include_directories(   ~/include/  ~/include/GraphicsEngine/)
add_executable(        chemistry      "${CHEM_PATH}/chemistry.cpp")
add_dependencies(      chemistry      GraphicsEngine)
target_link_libraries( chemistry      GraphicsEngine)

################################################################
#Installation step
################################################################
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "$ENV{HOME}/include/GraphicsEngine")
install(TARGETS   GraphicsEngine LIBRARY DESTINATION "$ENV{HOME}/lib/")
install(TARGETS   numero         RUNTIME DESTINATION "$ENV{HOME}/bin")
install(TARGETS   chemistry      RUNTIME DESTINATION "$ENV{HOME}/bin")

################################################################
#Creates header files from shaders
################################################################
add_custom_target(
  SHADERFILES
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" simple_frag_code   "../shaders/shader.frag"           simple_shader_frag.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" simple_vert_code   "../shaders/shader.vert"           simple_shader_vert.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" smooth_frag_code   "../shaders/smooth_shader.frag"    smooth_shader_frag.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" smooth_vert_code   "../shaders/smooth_shader.vert"    smooth_shader_vert.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" flat_frag_code     "../shaders/flat_shader.frag"      flat_shader_frag.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" flat_vert_code     "../shaders/flat_shader.vert"      flat_shader_vert.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" fresnel_frag_code  "../shaders/fresnel_shader.frag"   fresnel_shader_frag.h > /dev/null
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/embedfile" fresnel_vert_code  "../shaders/fresnel_shader.vert"   fresnel_shader_vert.h > /dev/null
  DEPENDS embedfile
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/include"
  COMMENT "Creates header files"
)
################################################################
#Allows to create .h files from .frag and .vert files
################################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
add_executable(                    embedfile sources/embedfile.cpp)
