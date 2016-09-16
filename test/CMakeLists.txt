CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (test_liter)

set (CMAKE_CXX_FLAGS "-std=c++14")

SET (TEST_SRC
  main.cpp
  utils_test/binaryTest.h
  utils_test/binaryTest.cpp
  GlobalEnv.h
  utils_test/singletonTest.h
  utils_test/singletonTest.cpp
  utils_test/stringUtilTest.h
  utils_test/stringUtilTest.cpp
  utils_test/rangeTest.h
  utils_test/rangeTest.cpp
  utils_test/sequenceTest.h
  utils_test/sequenceTest.cpp
  utils_test/utilsTest.cpp
  utils_test/utilsTest.h
  utils_test/callableTest.h
  utils_test/callableTest.cpp
  utils_test/lazyTest.h
  utils_test/lazyTest.cpp
  )

include_directories(../)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# boost
set(BOOST_ROOT "~/workspace/3rd/boost_1_61_0")
include_directories(${Boost_INCLUDE_DIRECTORY})

add_executable(main ${TEST_SRC})
target_link_libraries(main ${GTEST_LIBRARIES} pthread ${Boost_LIBRARIES})
