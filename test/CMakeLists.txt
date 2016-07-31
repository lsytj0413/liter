CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (test_liter)

set (CMAKE_CXX_FLAGS "-std=c++11")

SET (TEST_SRC
  main.cpp
  utils_test/binaryTest.h
  utils_test/binaryTest.cpp
  GlobalEnv.h
  utils_test/singletonTest.h
  utils_test/singletonTest.cpp
  )

include_directories(../)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(main ${TEST_SRC})
target_link_libraries(main ${GTEST_LIBRARIES} pthread)
