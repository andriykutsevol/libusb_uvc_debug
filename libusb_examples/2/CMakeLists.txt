 
project(HelloWorld)
cmake_minimum_required(VERSION 3.0)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FindLlibUSB1)

find_package(Threads REQUIRED)
set(threads Threads::Threads)


message(STATUS "LIBUSB1_INCLUDE_DIR: ${LIBUSB1_INCLUDE_DIR}")
message(STATUS "LIBUSB1_LIBRARY: ${LIBUSB1_LIBRARY}")


add_executable(example example.c)
target_include_directories(example PUBLIC ${LIBUSB1_INCLUDE_DIR})


target_link_libraries(example ${LIBUSB1_LIBRARY} ${threads})