CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(minitutorial_opencv)
FIND_PACKAGE(OpenCV 	REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(esqueleto src/esqueleto.cc)
add_executable(show_img src/show_img.cc)
add_executable(show_video src/show_video.cc)
add_executable(comp_stats src/comp_stats.cc)

#add_executable(pract1 pract1.cc)
#add_executable(pract2 pract2.cc)
#...

 
