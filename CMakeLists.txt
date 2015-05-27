cmake_minimum_required(VERSION 3.1)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(hw1 ${SOURCE_FILES})

IF (APPLE)
   include_directories (/System/Library/Frameworks)
   find_library (GLUT_LIBRARY GLUT)
   find_library (OpenGL_LIBRARY OpenGL)
   mark_as_advanced (GLUT_LIBRARY
                     OpenGL_LIBRARY)
   set (EXTRA_LIBS ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)

target_link_libraries(hw1 ${EXTRA_LIBS})