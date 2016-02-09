################################################################
# INITIALIZATION
################################################################
cmake_minimum_required(     VERSION 2.8)
set( CMAKE_BUILD_TYPE       "Release")
set( CMAKE_MACOSX_RPATH     1)

################################################################
#NumeRO application
################################################################
project(Numero)
set( NUMERO_PATH       "${CMAKE_SOURCE_DIR}/APP_NUMERO")
configure_file(        "${NUMERO_PATH}/numero.cpp" "${NUMERO_PATH}/numero.cpp")
include_directories(   ~/include/  ~/include/GraphicsEngine/)
add_executable(        numero      "${NUMERO_PATH}/numero.cpp")
add_dependencies(      numero      GraphicsEngine)
target_link_libraries( numero      GraphicsEngine)
################################################################
#Chemistry application
################################################################
project(Chemistry)
set( CHEM_PATH       "${CMAKE_SOURCE_DIR}/APP_CHEMISTRY")
configure_file(        "${CHEM_PATH}/chemistry.cpp" "${CHEM_PATH}/chemistry.cpp")
include_directories(   ~/include/  ~/include/GraphicsEngine/)
add_executable(        chemistry      "${CHEM_PATH}/chemistry.cpp")
add_dependencies(      chemistry      GraphicsEngine)
target_link_libraries( chemistry      GraphicsEngine)



################################################################
#Graphics Engine
################################################################
project( GraphicsEngine )
set( CMAKE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})
set( CMAKE_CXX_FLAGS        "-std=c++0x -g3 -O2 -w")
include_directories(        "~/include/" "include/" ${PNG_INCLUDE_DIR})
configure_file(             "${CMAKE_SOURCE_DIR}/include/cgl/defs.h.in" "${CMAKE_SOURCE_DIR}/include/cgl/defs.h")
################################################################
#Dependencies
################################################################
find_library( common NAMES Commons HINTS "$ENV{HOME}/lib")
find_package( GLUT REQUIRED)
find_package( OpenGL REQUIRED)
find_package( X11 REQUIRED)
find_package( GLEW REQUIRED)
set(CORELIBS  ${common} ${GLEW_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})
#if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
#	find_library( glew NAMES GLEW    HINTS "$ENV{HOME}/lib")
#	set(CORELIBS  ${common} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${X11_LIBRARIES} ${glew})
#endif()
################################################################
#Library
################################################################
file( GLOB_RECURSE     source_files sources/* include/cgl/* include/*)
add_library(           GraphicsEngine SHARED ${source_files})
target_link_libraries( GraphicsEngine ${CORELIBS})
add_dependencies(      GraphicsEngine SHADERFILES)



################################################################
#Installation step
################################################################
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "$ENV{HOME}/include/GraphicsEngine")
install(TARGETS   GraphicsEngine LIBRARY DESTINATION "$ENV{HOME}/lib/")
install(TARGETS   numero         RUNTIME DESTINATION "$ENV{HOME}/bin")
install(TARGETS   chemistry      RUNTIME DESTINATION "$ENV{HOME}/bin")
