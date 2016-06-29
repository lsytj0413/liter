CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (liter)

# ADD_SUBDIRECTORY(algo http utilsbin)

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
  utils/binary.h
  utils/sequence.h
  utils/Singleton.h
  utils/uncopyable.h
  utils/utils.h)

add_executable (liter ${LITER_SRC})
