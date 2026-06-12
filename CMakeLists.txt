cmake_minimum_required(VERSION 3.15...4.1)

project(ConvolveTest VERSION 1.0
        DESCRIPTION "Convolution Performance Tests."
        LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
endif()

set(OpenCV_DIR "/home/shawnshact/dev/opencv/build")
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui features2d videoio)
find_package(Eigen3 REQUIRED)

add_subdirectory(lib/convolve)

add_executable(ConvolveTest main.cpp)
target_compile_options(ConvolveTest PRIVATE -O0 -g)
target_compile_options(convolve PRIVATE -O0 -g)
target_link_libraries(ConvolveTest
        convolve
        ${OpenCV_LIBS}
        Eigen3::Eigen
)

target_include_directories(ConvolveTest PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)