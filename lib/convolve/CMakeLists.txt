add_library(convolve
    convolve.cpp
)

target_include_directories(convolve PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(convolve
    ${OpenCV_LIBS}
    Eigen3::Eigen
)
