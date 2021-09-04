cmake_minimum_required (VERSION 3.5)

project(Signature CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

if(UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif(UNIX)


# ENABLE WITH:  -DBUILD_TESTS=ON
option (BUILD_TESTS "Build tests" ON)

### external dependencies

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
if(BUILD_TESTS)
    find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)
    add_definitions(-DBOOST_TEST_DYN_LINK) 
else()
    find_package(Boost 1.71 REQUIRED)
endif()

### compiler-specific options
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_WIN32_WINNT=0x0A00) # Target is windows 10
    add_compile_options("/std:c++17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /MD")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /OPT:REF /OPT:ICF")
elseif(UNIX)
    find_package(Threads)
    add_definitions(-DNDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -std=c99 -Wall -D_DEFAULT_SOURCE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=c++17 -lrt -lpthread")
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

### source files
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

file(GLOB_RECURSE SIGNATURE_SRC
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_executable(${PROJECT_NAME} ${SIGNATURE_SRC})

target_sources(${PROJECT_NAME} PRIVATE ${SIGNATURE_SRC})

if(BUILD_TESTS)
    include(signature_tests.cmake)
endif()
