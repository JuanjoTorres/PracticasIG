cmake_minimum_required(VERSION 3.15)

project(practica1)

find_package(GLUT)
find_package(OpenGL)

set(CMAKE_CXX_STANDARD 14)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(practica1 Practica1/practica1.cpp)
target_link_libraries(practica1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica2)
add_executable(practica2 Practica2/practica2.cpp)
target_link_libraries(practica2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica3)
add_executable(pendulum Practica3/practica3.cpp)
target_link_libraries(pendulum ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(examen)
add_executable(examen Examen/examen.cpp)
target_link_libraries(examen ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica4)
add_executable(practica4 Practica4/practica4.cpp)
target_link_libraries(practica4 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica5)
add_executable(practica5 Practica5/practica5.cpp Practica5/Camera.cpp)
target_link_libraries(practica5 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(practica6)
add_executable(practica6 Practica6/practica6.cpp Practica6/Camera.cpp)
target_link_libraries(practica6 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

find_package(ZLIB)
find_package(assimp CONFIG REQUIRED)
find_package(SOIL CONFIG REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIRS})

project(practica7)
add_executable(practica7 Practica7/practica7.cpp Practica7/Camera.cpp)
target_link_libraries(practica7 PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ZLIB::ZLIB ${ASSIMP_LIBRARY_DIRS}/libIrrXML.a assimp::assimp SOIL)
