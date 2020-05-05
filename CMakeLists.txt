cmake_minimum_required(VERSION 3.15)

find_package(GLUT)
find_package(OpenGL)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

project(practica1)
add_executable(practica1 Practica1/practica1.cpp)
target_link_libraries(practica1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica2)
add_executable(practica2 Practica2/practica2.cpp)
target_link_libraries(practica2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica3)
add_executable(pendulum Practica3/practica3.cpp)
target_link_libraries(pendulum ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica4)
add_executable(practica4 Practica4/practica4.cpp)
target_link_libraries(pendulum ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

set(CMAKE_CXX_STANDARD 14)