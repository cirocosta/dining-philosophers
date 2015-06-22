ADD_SUBDIRECTORY(lib/)

ADD_EXECUTABLE(dinnp main.cpp)

TARGET_LINK_LIBRARIES(dinnp
  ${CMAKE_THREAD_LIBS_INIT}
  input
  food
  philosopher
)
