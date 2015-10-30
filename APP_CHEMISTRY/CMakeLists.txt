cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
set(PROJECT_NAME "Chemistry")
set(CMAKE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})

project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$ENV{HOME}/bin")

configure_file("${CMAKE_SOURCE_DIR}/app_chemistry.cpp" "${CMAKE_SOURCE_DIR}/app_chemistry.cpp")
include_directories("${CMAKE_SOURCE_DIR}/../build/library/glm/src/glm/"
		    "${CMAKE_SOURCE_DIR}/../include/")

add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/app_chemistry.cpp")
target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/../build/libSUscCGL.so")

#Penser à lancer "sudo /sbin/ldconfig -v" pour mettre à jour
