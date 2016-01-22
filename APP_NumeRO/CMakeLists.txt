cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR})
project(numero)

set(CMAKE_CXX_FLAGS "-std=c++0x -g3 -O2 -w")

configure_file("${CMAKE_SOURCE_DIR}/app_numero.cpp" "${CMAKE_SOURCE_DIR}/app_numero.cpp")
include_directories(~/include/ ~/include/SUscCGL/)

add_executable(numero "${CMAKE_SOURCE_DIR}/app_numero.cpp")
find_library(susccgl NAMES SUscCGL HINTS "$ENV{HOME}/lib")
target_link_libraries(numero ${susccgl})

install(TARGETS numero RUNTIME DESTINATION "$ENV{HOME}/bin")
