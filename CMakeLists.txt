cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(practica1)
add_executable(practica1 Practica1/practica1.cpp)

project(practica2)
add_executable(practica2 Practica2/practica2.cpp)

find_package(GLUT)
find_package(OpenGL)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(practica1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries(practica2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
set(CMAKE_CXX_STANDARD 14)