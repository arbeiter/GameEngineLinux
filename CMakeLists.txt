CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(project)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(/home/archie/glfw /home/archie/glfw/src)
# add_subdirectory(/home/archie/openal-soft /home/archie/openal-soft/src)
include( FindOpenAL )
include_directories( ${OPENAL_INCLUDE_DIR} )
FIND_PACKAGE(OpenGL REQUIRED)

SET(SOURCE_FILES main.cpp)

ADD_EXECUTABLE(project ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(project glfw ${OPENAL_LIBRARY})
TARGET_LINK_LIBRARIES(project OpenGL::GL)
