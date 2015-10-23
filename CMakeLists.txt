cmake_minimum_required(VERSION 2.8)

project(NeoQuad)

add_executable(NeoQuad main.cpp NeoQuad.cpp  Quadrotor.cpp  QuadTimer.cpp camera.cpp Timeline.cpp Button.cpp)


add_subdirectory(glm/)
include_directories(glm/)

#copy all files form Data/ to build/Data
#file(COPY Data/ DESTINATION Data/)
#install(TARGETS NeoQuad RUNTIME DESTINATION bin)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

#INCLUDE(FindFreeGLUT.cmake)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
#FIND_PACKAGE(FreeGLUT REQUIRED)

#ADD_DEFINITIONS(-DFREEGLUT_STATIC=1)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${FREEGLUT_INCLUDE_DIRS})
#SET(GLUT_LIB ${FREEGLUT_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${AgeofQuadrones_SOURCE_DIR})
link_directories(${NeoQuad_BINARY_DIR})


target_link_libraries(NeoQuad ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
