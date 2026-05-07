cmake_minimum_required(VERSION 4.3)

project(wBI_calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_executable(wbi ${SOURCES})

target_link_libraries(wbi PUBLIC OpenMP::OpenMP_CXX)

if (MSVC)
    target_compile_options(wbi PRIVATE /O2)
else()
    target_compile_options(wbi PRIVATE -O3)
endif()