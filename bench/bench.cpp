#include "convolve.hpp"
#include <benchmark/benchmark.h>
#include <opencv2/imgproc.hpp>
#include "bench.hpp"

#include <numeric>

using ConvFn = cv::Mat (*)(const cv::Mat&, const cv::Mat&);

namespace bench {
    std::vector<cv::Mat> generateTestImages() {
        std::vector<cv::Mat> images;
        for (int size: {128, 256, 512, 1024}) {
            cv::Mat img(size, size, CV_8UC1);
            cv::randu(img, 0, 255);
            images.push_back(img);
        }
        return images;
    }

    std::vector<cv::Mat> generateGaussianKernels() {
        std::vector<cv::Mat> kernels;
        for (int size : {3, 5, 7, 15, 31, 51, 101}) {
            for (double sigma : {1.0, 2.0, 5.0}) {
                cv::Mat k = cv::getGaussianKernel(size, sigma, CV_32F) *
                            cv::getGaussianKernel(size, sigma, CV_32F).t();
                kernels.push_back(k);
            }
        }
        return kernels;
    }

    std::vector<cv::Mat> generateBoxKernels() {
        std::vector<cv::Mat> kernels;
        for (int size : {3, 5, 7, 15, 31, 51, 101}) {
            cv::Mat k = cv::Mat::ones(size, size, CV_32F) / (size * size);
            kernels.push_back(k);
        }
        return kernels;
    }

    std::vector<cv::Mat> generateSobelKernels() {
        std::vector<cv::Mat> kernels;
        for (int size : {3, 5, 7, 15, 31, 51, 101}) {
            cv::Mat sobelX, sobelY;
            cv::getDerivKernels(sobelX, sobelX, 1, 0, size); // x derivative
            cv::getDerivKernels(sobelY, sobelY, 0, 1, size); // y derivative
            kernels.push_back(sobelX);
            kernels.push_back(sobelY);
        }
        return kernels;
    }

    std::vector<cv::Mat> generateTestKernels() {
        std::vector<cv::Mat> kernels;
        std::vector gaussianKernels = generateGaussianKernels();
        std::vector boxKernels = generateBoxKernels();
        // std::vector sobelKernels = generateSobelKernels();

        kernels.reserve(gaussianKernels.size() + boxKernels.size());
        kernels.insert(kernels.end(), gaussianKernels.begin(), gaussianKernels.end());
        kernels.insert(kernels.end(), boxKernels.begin(), boxKernels.end());
        // kernels.insert(kernels.end(), sobelKernels.begin(), sobelKernels.end());

        return kernels;
    }

    const std::vector<cv::Mat>& testImages() {
        static const std::vector<cv::Mat> images = generateTestImages();
        return images;
    }

    const std::vector<cv::Mat>& testKernels() {
        static const std::vector<cv::Mat> kernels = generateTestKernels();
        return kernels;
    }

    static void ConvArgs(benchmark::internal::Benchmark* b) {
        std::vector<int64_t> imageSizes(testImages().size());
        std::vector<int64_t> kernelSizes(testKernels().size());
        std::iota(imageSizes.begin(), imageSizes.end(), 0);
        std::iota(kernelSizes.begin(), kernelSizes.end(), 0);
        b->ArgsProduct({imageSizes, kernelSizes});
    }

    static void BM_Conv(benchmark::State &state, ConvFn fn)
    {
        const cv::Mat& image  = testImages()[state.range(0)];
        const cv::Mat& kernel = testKernels()[state.range(1)];
        state.SetLabel(std::to_string(image.cols) + "x" + std::to_string(image.rows) + " k" + std::to_string(kernel.cols));
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(fn(image, kernel));
        }
    }

    BENCHMARK_CAPTURE(BM_Conv, baseline, conv::baselineConv) -> Apply(ConvArgs) -> Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Conv, separable, conv::separableConv) -> Apply(ConvArgs) -> Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Conv, fft, conv::fftConv) -> Apply(ConvArgs) -> Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Conv, sliding, conv::slidingWindowConv) -> Apply(ConvArgs) -> Unit(benchmark::kMillisecond);
}