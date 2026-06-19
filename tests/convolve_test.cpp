#include "convolve.hpp"
#include <gtest/gtest.h>
#include <opencv2/core.hpp>

/* Test cases
 * Correctness (known answer):
 *   - identity kernel (returns input)
 *   - shift kernel (known offset)
 *   - hand-computed 3x3 example
 * Properties (random input):
 *   - FFT conv ≈ naive conv  <-- most important here
 *   - commutativity, linearity
 *   - constant input + normalized kernel
 * Kernels: box, Gaussian, edge (signed, sums to 0)
 * Boundaries: zero-pad / reflect / replicate; corners & edges
 * Sizes: 1x1, even, larger-than-image, non-square, several conv sizes
 */
cv::Mat makeRandomMatrix(int rows, int cols, int type, uint64 seed) {
    cv::Mat m(rows, cols, type);
    cv::RNG rng(seed);
    rng.fill(m, cv::RNG::UNIFORM, cv::Scalar(0), cv::Scalar(255));
    return m;
}

struct ConvCase {
    std::string name;
    std::function<cv::Mat(const cv::Mat&, const cv::Mat&)> fn;
    double tol;
};

class CorrectnessTests : public ::testing::TestWithParam<ConvCase> {};

TEST_P(CorrectnessTests, IdentityKernelShouldNotChangeInput) {
    // Setup
    const auto& [name, conv, tol] = GetParam();
    std::vector<float> identityKernelData = {0, 0, 0, 0, 1, 0, 0, 0, 0 };
    const cv::Mat identityKernel(3,3, CV_32F, identityKernelData.data());
    const cv::Mat testImage = makeRandomMatrix(16, 16, CV_8UC1, 0);
    const cv::Mat& expected = testImage;

    // Execute
    const cv::Mat result = conv(testImage, identityKernel);

    // Evaluate
    ASSERT_EQ(result.size(), testImage.size());
    ASSERT_EQ(result.type(), testImage.type());
    EXPECT_EQ(cv::norm(result, expected, cv::NORM_INF), tol);
}

TEST_P(CorrectnessTests, HorizontalShiftKernelShouldTranslateImage) {
    // Setup
    const auto& [name, conv, tol] = GetParam();
    std::vector<float> HorizontalShiftKernelData = {0, 0, 0, 1, 0, 0, 0, 0, 0 };
    const::cv::Mat HorizontalShiftKernel(3, 3, CV_32F, HorizontalShiftKernelData.data());
    const cv::Mat testImage = makeRandomMatrix(5, 7, CV_8UC1, 0);
    cv::Mat expected = testImage(cv::Rect(0, 0, testImage.cols - 1, testImage.rows));
    cv::copyMakeBorder(expected, expected, 0, 0, 1, 0, cv::BORDER_CONSTANT);

    // Execute
    cv::Mat result = conv(testImage, HorizontalShiftKernel);

    // Evaluate
    ASSERT_EQ(result.size(), testImage.size());
    ASSERT_EQ(result.type(), testImage.type());
    EXPECT_EQ(cv::norm(result, expected, cv::NORM_INF), tol);
}

TEST_P(CorrectnessTests, VerticalShiftKernelShouldTranslateImage) {
    // Setup
    const auto& [name, conv, tol] = GetParam();
    std::vector<float> HorizontalShiftKernelData = {0, 1, 0, 0, 0, 0, 0, 0, 0 };
    const::cv::Mat HorizontalShiftKernel(3, 3, CV_32F, HorizontalShiftKernelData.data());
    const cv::Mat testImage = makeRandomMatrix(5, 7, CV_8UC1, 0);
    cv::Mat expected = testImage(cv::Rect(0, 0, testImage.cols, testImage.rows-1));
    cv::copyMakeBorder(expected, expected, 1, 0, 0, 0, cv::BORDER_CONSTANT);

    // Execute
    cv::Mat result = conv(testImage, HorizontalShiftKernel);

    // Evaluate
    ASSERT_EQ(result.size(), testImage.size());
    ASSERT_EQ(result.type(), testImage.type());
    EXPECT_EQ(cv::norm(result, expected, cv::NORM_INF), tol);
}

INSTANTIATE_TEST_SUITE_P(
    AllImpls, CorrectnessTests,
    ::testing::Values(
        ConvCase{"Baseline",conv::baselineConv,0.0},
        ConvCase{"SlidingWindow", conv::slidingWindowConv, 0.0},
        ConvCase{"Separable",conv::separableConv,0.0},
        ConvCase{"Fft",conv::fftConv,0}
    ),
    [](const auto& info){ return info.param.name; }  // readable test names
);


