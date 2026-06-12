#pragma once
#include <cstdint>
#include <opencv2/core/mat.hpp>

namespace conv {
    enum class Direction : std::uint8_t { X, Y };

    cv::Mat getSobelX(int size, Direction direction);

    cv::Mat baselineConv(const cv::Mat &image, const cv::Mat &kernel);
    cv::Mat slidingWindowConv(const cv::Mat &image, const cv::Mat &kernel);
    cv::Mat separableConv(const cv::Mat &image, const cv::Mat &kernel);
    cv::Mat fftConv(const cv::Mat &image, const cv::Mat &kernel);
};