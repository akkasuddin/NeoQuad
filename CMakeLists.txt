cmake_minimum_required(VERSION 2.8)

project(NeoQuad)

add_executable(NeoQuad main.cpp NeoQuad.cpp  Quadrotor.cpp  QuadTimer.cpp camera.cpp Timeline.cpp Button.cpp  ImageLoader/bmpIO.cpp ImageLoader/pgmIO.cpp)


add_subdirectory(glm/)
include_directories(glm/)

#copy all files form Data/ to build/Data
#file(COPY Data/ DESTINATION Data/)
add_custom_command(TARGET NeoQuad PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Data $<TARGET_FILE_DIR:NeoQuad>/Data)

#install(TARGETS NeoQuad RUNTIME DESTINATION bin)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package (Threads REQUIRED)

#INCLUDE(FindFreeGLUT.cmake)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
#FIND_PACKAGE(FreeGLUT REQUIRED)

#ADD_DEFINITIONS(-DFREEGLUT_STATIC=1)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${FREEGLUT_INCLUDE_DIRS})
#SET(GLUT_LIB ${FREEGLUT_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${NeoQuad_SOURCE_DIR})
link_directories(${NeoQuad_BINARY_DIR} )


target_link_libraries(NeoQuad ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}  ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_SOURCE_DIR}/lib/libSOIL.a)
