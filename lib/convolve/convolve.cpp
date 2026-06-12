#ifndef CONVOLVETEST_LIB_CONVOLVE_H_
#define CONVOLVETEST_LIB_CONVOLVE_H_

#include "convolve.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>

namespace conv {

cv::Mat getSobelKernel(int size, Direction direction) {
    // TODO Implement later
    cv::Mat output;
    return output;

}

cv::Mat baselineConv(const cv::Mat &image, const cv::Mat &kernel) {
    cv::Mat output;
    cv::filter2D(image, output, -1, kernel);
    return output;
}

cv::Mat slidingWindowConv(const cv::Mat &image, const cv::Mat &kernel) {
    const int padding = kernel.rows/2;
    cv::Mat paddedImage;
    image.convertTo(paddedImage, CV_32F);
    cv::copyMakeBorder(paddedImage, paddedImage, padding, padding, padding, padding, cv::BORDER_CONSTANT);
    cv::Mat output = cv::Mat::zeros(image.rows, image.cols, CV_32F);

    // TODO: Embarrassingly parallel, optimize
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            output.at<float>(i,j) = paddedImage(cv::Rect(j, i, kernel.cols, kernel.rows)).dot(kernel);
        }
    }

    output.convertTo(output, CV_8UC1);
    return output;
}

cv::Mat separableConv(const cv::Mat &image, const cv::Mat &kernel) {
    // Idea: Decompose kernel into SVD and try to compute vh^T
    // Convolve each column with v, then convolve each row with h

    // Pad image
    const int padding = kernel.rows/2;
    cv::Mat paddedImage;
    image.convertTo(paddedImage, CV_32F);
    cv::copyMakeBorder(paddedImage, paddedImage, padding, padding, 0, 0, cv::BORDER_CONSTANT);

    // Create output image
    cv::Mat output1 = cv::Mat::zeros(image.rows, image.cols, CV_32F);
    cv::Mat output2 = cv::Mat::zeros(image.rows, image.cols, CV_32F);

    // Do SVD to get vh^T
    cv::SVD kernelSvd(kernel);
    float sqrt_sigma = std::sqrt(kernelSvd.w.at<float>(0,0));
    cv::Mat v = sqrt_sigma*kernelSvd.u.col(0);
    cv::Mat ht = sqrt_sigma*kernelSvd.vt.row(0);

    // Convolve v with columns of I and store in output1
    for (int c = 0; c < image.cols; c++) {
        for (int r = 0; r < image.rows; r++) {
            output1.at<float>(r,c) = paddedImage.col(c).rowRange(r, r+v.rows).dot(v);
        }
    }
    cv::copyMakeBorder(output1, output1, 0, 0, padding, padding, cv::BORDER_CONSTANT);
    // Create output image
    cv::Mat output_v = cv::Mat::zeros(image.rows, image.cols, CV_32F);
    //
    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            output2.at<float>(r,c) = output1.row(r).colRange(c, c+ht.cols).dot(ht);
        }
    }
    output2.convertTo(output2, CV_8UC1);
    return output2;
}

cv::Mat fftConv(const cv::Mat &image, const cv::Mat &kernel) {
    /* TODO: Implement DFT using FFT
     * Step 1: Use OpenCV DFT function to understand how to apply DFT to an image.
     * Step 2: Naively compute DFT using standard formula .
     * Step 3: Implement FFT to compute DFT.
     */
    cv::Mat output;
    cv::Mat imageFreq;
    cv::Mat kernelFreq;
    image.convertTo(imageFreq, CV_32FC1);
    kernel.convertTo(kernelFreq, CV_32FC1);
    cv::dft(imageFreq, imageFreq);
    cv::dft(kernelFreq, kernelFreq);
    return output;

}
} // namespace conv

#endif // CONVOLVETEST_LIB_CONVOLVE_H_
