cmake_minimum_required(VERSION 3.15...4.1)

project(ConvolveTest VERSION 1.0
        DESCRIPTION "Convolution Performance Tests."
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "" FORCE)
endif()

# Per-target opt-in. ON by default only when this is the top-level project.
option(CONVOLVE_BUILD_DEMO       "Build demo apps"   ${PROJECT_IS_TOP_LEVEL})
option(CONVOLVE_BUILD_TESTS      "Build tests"       ${PROJECT_IS_TOP_LEVEL})
option(CONVOLVE_BUILD_BENCHMARKS "Build benchmarks"  ${PROJECT_IS_TOP_LEVEL})

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui features2d videoio)
find_package(Eigen3 REQUIRED)

add_subdirectory(lib/convolve)          # always: the core library

if(CONVOLVE_BUILD_DEMO)
    add_subdirectory(apps)
endif()

if(CONVOLVE_BUILD_TESTS)
    enable_testing()                    # registers `ctest`
    add_subdirectory(tests)
endif()

if(CONVOLVE_BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()