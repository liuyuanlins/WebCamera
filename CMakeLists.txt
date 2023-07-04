cmake_minimum_required(VERSION 3.10)
project(Demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/WebServer.cpp)
link_libraries(wfrest)
add_executable(WebServer ${SRC})

find_package(OpenCV REQUIRED)
file(GLOB SRC2 ${CMAKE_CURRENT_SOURCE_DIR}/RTSPLiveCapture.cpp)
add_executable(RTSPLiveCapture ${SRC2})
target_link_libraries(RTSPLiveCapture pthread ${OpenCV_LIBS})

file(GLOB SRC3 ${CMAKE_CURRENT_SOURCE_DIR}/RTSPVideoStreamer.cpp)
add_executable(RTSPVideoStreamer ${SRC3})
target_link_libraries(RTSPVideoStreamer pthread ${OpenCV_LIBS})

file(GLOB SRC4 ${CMAKE_CURRENT_SOURCE_DIR}/ImageProcesser.cpp)
add_executable(ImageProcesser ${SRC4})
target_link_libraries(ImageProcesser pthread ${OpenCV_LIBS})