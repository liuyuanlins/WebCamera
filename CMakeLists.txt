cmake_minimum_required(VERSION 3.10)
project(Demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
find_package(OpenCV REQUIRED)


file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
link_libraries(wfrest)
add_executable(WebServer ${SRC})
target_link_libraries(WebServer pthread ${OpenCV_LIBS})
