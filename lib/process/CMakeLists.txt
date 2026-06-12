add_library(process
    process.cpp
)

target_include_directories(convolve PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
