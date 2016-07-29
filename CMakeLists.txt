CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (liter)

set (CMAKE_CXX_FLAGS "-std=c++11")
include_directories(./)


add_subdirectory(liter bin)
