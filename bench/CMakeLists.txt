find_package(benchmark REQUIRED)        # google-benchmark

add_executable(convolve_bench bench.cpp)
target_link_libraries(convolve_bench PRIVATE convolve::convolve benchmark::benchmark_main)
