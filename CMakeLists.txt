cmake_minimum_required(VERSION 3.0)
option(REFLECT_BUILD_TESTS "Build reflect tests" ON)
set (CMAKE_CXX_STANDARD 11)
project(reflect)

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SRC "src/*.cc")

add_library(lib ${SRC})
set_target_properties(lib PROPERTIES OUTPUT_NAME "reflect")

if(REFLECT_BUILD_TESTS)
  set(tests basic)
  foreach(test ${tests})
    add_executable(${test} test/${test}.cc)
    target_link_libraries(${test} lib)
    install(TARGETS ${test} DESTINATION bin)
  endforeach()
endif()
#set(LIBRARY_OUTPUT_PATH ${HOME}/lib)
