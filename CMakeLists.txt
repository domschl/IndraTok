cmake_minimum_required (VERSION 3.24.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (IndraTokHome)

add_definitions(-DIA_STACK_PREFERENCE=32)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
  
add_subdirectory (IndraTok)
add_subdirectory (IndraAtomTest)
add_subdirectory (IndraTokTest)

set_property(TARGET IndraTok PROPERTY C_STANDARD 11)
