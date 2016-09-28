CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (test_liter)

set (CMAKE_CXX_FLAGS "-std=c++14")

SET (TEST_SRC
  main.cpp
  GlobalEnv.h
  utilsTest/binaryTest.h
  utilsTest/binaryTest.cpp
  utilsTest/singletonTest.h
  utilsTest/singletonTest.cpp
  utilsTest/stringUtilTest.h
  utilsTest/stringUtilTest.cpp
  utilsTest/rangeTest.h
  utilsTest/rangeTest.cpp
  utilsTest/sequenceTest.h
  utilsTest/sequenceTest.cpp
  utilsTest/utilsTest.cpp
  utilsTest/utilsTest.h
  utilsTest/callableTest.h
  utilsTest/callableTest.cpp
  utilsTest/lazyTest.h
  utilsTest/lazyTest.cpp
  utilsTest/functionTraitsTest.h
  utilsTest/functionTraitsTest.cpp
  utilsTest/anyTest.h
  utilsTest/anyTest.cpp
  utilsTest/aspectTest.h
  utilsTest/aspectTest.cpp
  utilsTest/commandTest.cpp
  utilsTest/commandTest.h
  utilsTest/eventsTest.h
  utilsTest/eventsTest.cpp
  utilsTest/iocTest.h
  utilsTest/iocTest.cpp
  utilsTest/messageBusTest.h
  utilsTest/messageBusTest.cpp
  utilsTest/objectPoolTest.h
  utilsTest/objectPoolTest.cpp
  utilsTest/optionalTest.h
  utilsTest/optionalTest.cpp
  utilsTest/visitorTest.h
  utilsTest/visitorTest.cpp
  utilsTest/variantTest.h
  utilsTest/variantTest.cpp
  utilsTest/tupleHelperTest.h
  utilsTest/tupleHelperTest.cpp
  utilsTest/timerTest.h
  utilsTest/timerTest.cpp
  utilsTest/threadPoolTest.h
  utilsTest/threadPoolTest.cpp
  utilsTest/taskTest.h
  utilsTest/taskTest.cpp
  utilsTest/linqTest.h
  utilsTest/linqTest.cpp
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
