cmake_minimum_required(VERSION 3.12)
project(toaster)

set(CMAKE_CXX_STANDARD 20)

function(find_and_link target library_name)
    find_library(LIB${library_name}_PATH "${library_name}")
    message(STATUS "Using ${library_name} ${LIB${library_name}_PATH}")
    target_link_libraries(${target} PRIVATE "${LIB${library_name}_PATH}")
endfunction(find_and_link)

add_library(max31856 max31856/max31856.c)
target_include_directories(max31856 PUBLIC max31856/)

add_library(toasterlib toasterlib/toaster.c)
target_include_directories(toasterlib PUBLIC toasterlib/)
target_include_directories(toasterlib PUBLIC max31856/)

find_and_link(max31856 bcm2835)
