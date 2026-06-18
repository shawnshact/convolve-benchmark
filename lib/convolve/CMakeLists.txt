add_library(convolve convolve.cpp)
add_library(convolve::convolve ALIAS convolve)   # namespaced alias = the standard idiom

target_include_directories(convolve PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(convolve PUBLIC ${OpenCV_LIBS} Eigen3::Eigen)
target_include_directories(convolve PUBLIC ${OpenCV_INCLUDE_DIRS})