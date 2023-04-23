cmake_minimum_required(VERSION 3.0.0)
project(Labyrinths VERSION 0.1.0)



include(CTest)
enable_testing()

add_library(Generator INTERFACE)
target_include_directories(Generator INTERFACE include/)

add_executable(Labyrinths src/main.cpp src/Labyrinths.cpp)
target_link_libraries(Labyrinths PRIVATE Generator)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
