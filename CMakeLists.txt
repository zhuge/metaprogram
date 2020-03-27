cmake_minimum_required (VERSION 3.15)

project(metatprogram LANGUAGES CXX VERSION 0.0.1)

option(METAPROGRAM_BUILD_TESTING "Build metaprogram with unittests" ON)

# c/cxx standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

# msvc libraries
if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
endif ()

# include from src 
include_directories(${LUA_INCLUDE_DIR} ./src)

# library target
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
# unittests target

if (METAPROGRAM_BUILD_TESTING)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()
