find_package(GTest REQUIRED)            # or FetchContent
include(GoogleTest)

add_executable(convolve_tests convolve_test.cpp)
target_link_libraries(convolve_tests PRIVATE convolve::convolve GTest::gtest_main)
gtest_discover_tests(convolve_tests)    # auto-registers each TEST() with CTest