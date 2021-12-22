CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(p1)
FIND_PACKAGE(OpenCV 	REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")
set(SOURCE_FILES src/main.cc src/funciones.cc)

add_executable(realce ${SOURCE_FILES})
