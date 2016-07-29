CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (liter)

# ADD_SUBDIRECTORY(algo http utilsbin)

include_directories(./)

SET (LITER_SRC
  liter/algo/heap_sort.h
  liter/algo/max_heap.h
  liter/algo/min_heap.h
  liter/algo/minmax_heap_imp.h
  liter/algo/tree_utils.h
  liter/http/CurlHandle.cpp
  liter/http/CurlHandle.h
  liter/http/HttpClient.cpp
  liter/http/HttpClient.h
  liter/http/HttpRequest.cpp
  liter/http/HttpRequest.h
  liter/http/HttpResponse.cpp
  liter/http/HttpResponse.h
  liter/utils/binary.h
  liter/utils/sequence.h
  liter/utils/Singleton.h
  liter/utils/uncopyable.h
  liter/utils/utils.h
  )

add_executable (liter ${LITER_SRC})
