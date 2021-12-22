CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(blur_background)
ENABLE_LANGUAGE(CXX)

FIND_PACKAGE(OpenCV 3.0	REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(blur_background blur_background.cpp)


 
