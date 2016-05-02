cmake_minimum_required (VERSION 3.1)
project (test1)

set (CMAKE_BUILD_TYPE debug)

set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/vboIndexer.cpp)
set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/console.cpp)

include_directories (${PROJECT_SOURCE_DIR}/src/objects/)
set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/objects/object.cpp)
set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/objects/windmill.cpp)

include_directories (${PROJECT_SOURCE_DIR}/src/assets/)
set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/assets/objLoader.cpp)
set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/assets/mtlLoader.cpp)
set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/assets/image.cpp)
#set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/src/assets/font.cpp)

include_directories (${PROJECT_SOURCE_DIR}/include/)

#Include LodePNG:
include_directories ("${PROJECT_SOURCE_DIR}/include/LodePNG")
add_subdirectory ("include/LodePNG")
set (EXTRA_LIBS ${EXTRA_LIBS} "LodePNG")

#include FreeType
#include_directories ("${PROJECT_SOURCE_DIR}/include/FreeType")
#set (EXTRA_SRC ${EXTRA_SRC} ${PROJECT_SOURCE_DIR}/include/FreeType/libfreetype.a)
#set (EXTRA_LIBS ${EXTRA_LIBS} ${PROJECT_SOURCE_DIR}/include/FreeType/libfreetype.a)

#Include OpenGL and GLEW:
find_package (OpenGL REQUIRED)
include_directories (OPENGL_INCLUDE_DIR)
find_package (GLEW REQUIRED)
include_directories (GLEW_INCLUDE_DIR)

#Include GLFW:
find_package (PkgConfig REQUIRED)
pkg_search_module (GLFW REQUIRED glfw3)
include_directories (${GLFW_INCLUDE_DIRS})

#Documentation:
find_package (Doxygen)
if (DOXYGEN_FOUND)
  add_custom_target (docs ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/docs/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif (DOXYGEN_FOUND)

#Executable info:
add_executable (weather ${PROJECT_SOURCE_DIR}/src/main.cpp ${EXTRA_SRC})
set_property (TARGET weather PROPERTY CXX_STANDARD 11)
set_property (TARGET weather PROPERTY CXX_STANDARD_REQUIRED ON)
set_property (TARGET weather PROPERTY OUTPUT_NAME "weather.out")
target_link_libraries (weather ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${PNG_LIBRARIES} ${EXTRA_LIBS} )
