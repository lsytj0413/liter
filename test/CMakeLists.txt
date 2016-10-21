CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (test_liter)

set (CMAKE_CXX_FLAGS "-std=c++14")

SET (TEST_SRC
  main.cpp
  GlobalEnv.h
  utilsTest/binaryTest.cpp
  utilsTest/singletonTest.cpp
  utilsTest/stringUtilTest.cpp
  utilsTest/rangeTest.cpp
  utilsTest/sequenceTest.cpp
  utilsTest/utilsTest.cpp
  utilsTest/callableTest.cpp
  utilsTest/lazyTest.cpp
  utilsTest/functionTraitsTest.cpp
  utilsTest/anyTest.cpp
  utilsTest/aspectTest.cpp
  utilsTest/commandTest.cpp
  utilsTest/eventsTest.cpp
  utilsTest/iocTest.cpp
  utilsTest/messageBusTest.cpp
  utilsTest/objectPoolTest.cpp
  utilsTest/optionalTest.cpp
  utilsTest/visitorTest.cpp
  utilsTest/variantTest.cpp
  utilsTest/tupleHelperTest.cpp
  utilsTest/timerTest.cpp
  utilsTest/threadPoolTest.cpp
  utilsTest/taskTest.cpp
  utilsTest/linqTest.cpp
  utilsTest/curryTest.cpp
  algoTest/matchTest.cpp
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
