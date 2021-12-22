#pragma once
#include <opencv2/core.hpp>

/**
 * @brief Return a Gaussian filter.
 * @arg[in] r is the filter's radius.
 * @return the filter.
 * @pre r>0;
 * @post ret_v.type()==CV_32FC1
 * @post retV.rows==retV.cols==2*r+1
 * @post (abs(cv::sum(retV)-1.0)<1.0e-6
 */
cv::Mat fsiv_create_gaussian_filter(const int r);


/**
 * @brief Create a laplacian filter.
 * @param filter_type 0->LAP_4, 1->LAP_8, 2->DoG.
 * @param r1 if filter type is DoG, r1 is the radius for the first Gaussian filter.
 * @param r2 if filter type is DoG, r2 is the radius for the second Gaussian filter.
 * @return the filter.
 * @post retval.type()==CV_32FC1
 */
cv::Mat fsiv_create_laplacian_filter(const int filter_type, int r1=1, int r2=2);

/**
 * @brief Expand an image with zero padding.
 * @warning the code can't use the interface cv::copyMakeborder().
 * @arg[in] in is the input image.
 * @arg[in] r is the window's radius to expand.
 * @return the expanded image.
 * @pre !in.empty()
 * @pre r>0
 * @post retV.type()==in.type()
 * @post retV.rows==in.rows+2*r
 * @post retV.cols==in.cols+2*r
 */
cv::Mat fsiv_fill_expansion(cv::Mat const& in, const int r);

/**
 * @brief Circular expansion of an image.
 * @warning the code can't use the interface cv::copyMakeborder().
 * @arg[in] in is the input image.
 * @arg[in] r is the window's radius to expand.
 * @return the expanded image.
 * @pre !in.empty()
 * @pre r>0
 * @post retV.type()==in.type()
 * @post retV.rows==in.rows+2*r
 * @post retV.cols==in.cols+2*r
 */
cv::Mat fsiv_circular_expansion(cv::Mat const& in, const int r);


/**
 * @brief Compute the digital correlation between two images.
 * @warning Code from scracth. Use cv::filter2D() is not allowed.
 * @arg[in] in is the input image.
 * @arg[in] filter is the filter to be applied.
 * @pre !in.empty() && !filter.empty()
 * @pre in.type()==CV_32FC1 && filter.type()==CV_32FC1.
 * @post ret.type()==CV_32FC1
 * @post ret.rows == in.rows-2*(filters.rows/2)
 * @post ret.cols == in.cols-2*(filters.cols/2)
 */
cv::Mat fsiv_filter2D(cv::Mat const& in, cv::Mat const& filter);

/**
 * @brief Extend an image with zeros.
 * The original image is centered in the new one.
 * @param in the input image.
 * @param new_size is the new geometry.
 * @return the extended image.
 * @pre img.rows<new_size.height
 * @pre img.cols<new_size.width
 * @post out.type()==img.type()
 * @post out.rows == new_size.height
 * @post out.cols == new_size.width
 */
cv::Mat fsiv_extend_image(const cv::Mat& img, const cv::Size& new_size);

/**
 * @brief Do a sharpeing enhance to an image.
 * @param img is the input image.
 * @param gain is the enhance's gain.
 * @param filter_type is the filter to use: 0->LAP_4, 1->LAP_8, 2->DOG.
 * @param only_luma if the input image is RGB only enhances the luma, else enhances all RGB channels.
 * @param r1 if filter type is DOG, is the radius of first Gaussian filter.
 * @param r2 if filter type is DOG, is the radius of second Gaussian filter.
 * @param circular if it is true, use circular convolution.
 * @return the enahance image.
 * @pre filter_type in {0,1,2}.
 * @pre 0<r1<r2
 */
cv::Mat fsiv_image_sharpening(const cv::Mat& in, float gain, int filter_type, bool only_luma,
                      int r1, int r2, bool circular);
