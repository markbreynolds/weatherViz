cmake_minimum_required (VERSION 3.1)
project (test1)

set (CMAKE_BUILD_TYPE debug)
set (EXTRA_SRC ${PROJECT_SOURCE_DIR}/src/object.cpp)

include_directories (${PROJECT_SOURCE_DIR}/include/)
# Dont need LodePNG yet...
#include_directories ("${PROJECT_SOURCE_DIR}/LodePNG")
#add_subdirectory ("LodePNG")
#set (EXTRA_LIBS ${EXTRA_LIBS} "LodePNG")

#set(EXTRA_LIBS ${EXTRA_LIBS} glew glfw3 glm)
find_package (OpenGL REQUIRED)
include_directories (OPENGL_INCLUDE_DIR)
find_package (GLEW REQUIRED)
include_directories (GLEW_INCLUDE_DIR)
find_package (PNG REQUIRED)
include_directories (PNG_INCLUDE_DIR)

find_package (PkgConfig REQUIRED)
pkg_search_module (GLFW REQUIRED glfw3)
include_directories ($(GLFW_INCLUDE_DIRS))

add_executable (weather ${PROJECT_SOURCE_DIR}/src/main.cpp ${EXTRA_SRC})
set_property (TARGET weather PROPERTY CXX_STANDARD 11)
set_property (TARGET weather PROPERTY CXX_STANDARD_REQUIRED ON)
set_property (TARGET weather PROPERTY OUTPUT_NAME "weather.out")
target_link_libraries (weather ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${PNG_LIBRARIES} ${EXTRA_LIBS} )
