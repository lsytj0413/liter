
set (CMAKE_CXX_FLAGS "-std=c++14")

SET (LITER_SRC
  algo/heap_sort.h
  algo/max_heap.h
  algo/min_heap.h
  algo/minmax_heap_imp.h
  algo/tree_utils.h
  http/CurlHandle.cpp
  http/CurlHandle.h
  http/HttpClient.cpp
  http/HttpClient.h
  http/HttpRequest.cpp
  http/HttpRequest.h
  http/HttpResponse.cpp
  http/HttpResponse.h
  binary.hpp
  sequence.hpp
  Singleton.h
  uncopyable.h
  utils.h
  )

# boost
set(BOOST_ROOT "~/workspace/3rd/boost_1_61_0")
include_directories(${Boost_INCLUDE_DIRECTORY})


include_directories(../)
add_executable (liter ${LITER_SRC})

target_link_libraries(liter ${Boost_LIBRARIES})
