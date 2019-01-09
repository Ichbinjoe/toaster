cmake_minimum_required(VERSION 3.10)
project(toaster)

set(CMAKE_CXX_STANDARD 20)

function(find_and_link target library_name)
    find_library(LIB${library_name}_PATH "${library_name}")
    message(STATUS "Using ${library_name} ${LIB${library_name}_PATH}")
    target_link_libraries(${target} PRIVATE "${LIB${library_name}_PATH}")
endfunction(find_and_link)

add_library(toaster SHARED src/toaster.c src/max31856.c)
target_include_directories(toaster PUBLIC include/)

find_and_link(toaster bcm2835)
