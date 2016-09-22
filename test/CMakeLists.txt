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
  utils_test/functionTraitsTest.h
  utils_test/functionTraitsTest.cpp
  utils_test/anyTest.h
  utils_test/anyTest.cpp
  utils_test/aspectTest.h
  utils_test/aspectTest.cpp
  utils_test/commandTest.cpp
  utils_test/commandTest.h
  utils_test/eventsTest.h
  utils_test/eventsTest.cpp
  utils_test/iocTest.h
  utils_test/iocTest.cpp
  utils_test/messageBusTest.h
  utils_test/messageBusTest.cpp
  utils_test/objectPoolTest.h
  utils_test/objectPoolTest.cpp
  utils_test/optionalTest.h
  utils_test/optionalTest.cpp
  utils_test/visitorTest.h
  utils_test/visitorTest.cpp
  utils_test/variantTest.h
  utils_test/variantTest.cpp
  utils_test/tupleHelperTest.h
  utils_test/tupleHelperTest.cpp
  )

include_directories(../)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

SET( CMAKE_BUILD_TYPE Debug )

# boost
set(BOOST_ROOT "~/workspace/3rd/boost_1_61_0")
include_directories(${Boost_INCLUDE_DIRECTORY})

add_executable(main ${TEST_SRC})
target_link_libraries(main ${GTEST_LIBRARIES} pthread ${Boost_LIBRARIES})
