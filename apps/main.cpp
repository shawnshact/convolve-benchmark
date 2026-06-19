#include "convolve.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>

int main() {
    const cv::Mat testImage =
        cv::imread("images/1.png", cv::IMREAD_GRAYSCALE);
    std::vector<float> sobelXData = {-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0};
    const cv::Mat sobelX(3, 3, CV_32F, sobelXData.data());

    const cv::Mat refConvImage = conv::baselineConv(testImage, sobelX);
    const cv::Mat slidingWindowConvImage = conv::slidingWindowConv(testImage, sobelX);
    const cv::Mat separableConvImage = conv::separableConv(testImage, sobelX);
    const cv::Mat fftConvImage = conv::fftConv(testImage, sobelX);
    const std::vector images = {refConvImage, slidingWindowConvImage, separableConvImage, fftConvImage};

    cv::Mat displayImg;
    cv::hconcat(images, displayImg);
    cv::imshow("Window", displayImg);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}