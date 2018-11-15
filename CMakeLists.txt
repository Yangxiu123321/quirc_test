cmake_minimum_required(VERSION 3.0)

# Project Name
set(PROJ_NAME quirc_test)
project(${PROJ_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

FILE(GLOB HEADER_LIST "${CMAKE_SOURCE_DIR}/src/*.h")
include_directories("${CMAKE_SOURCE_DIR}/src")
FILE(GLOB SOURCE_LIST "${CMAKE_SOURCE_DIR}/src/*.c")
FILE(GLOB SOURCE_CPP_LIST "${CMAKE_SOURCE_DIR}/src/*.cpp")


add_executable(${PROJ_NAME} ${SOURCE_LIST} ${HEADER_LIST} ${SOURCE_CPP_LIST})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})

# OpenCV
find_package(OpenCV)
target_link_libraries(${PROJ_NAME} ${OpenCV_LIBS})