cmake_minimum_required(VERSION 3.14)
project(AE)

set(PEGTL_BUILD_EXAMPLES OFF CACHE BOOL "Nope" FORCE)
set(PEGTL_BUILD_TESTS OFF CACHE BOOL "Nope" FORCE)
add_subdirectory(PEGTL)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp AE/Grammar.hpp AE/Action.hpp)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC taocpp::pegtl)