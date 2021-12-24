#include <iostream>
#include "common_code.hpp"
#include <opencv2/imgproc.hpp>

cv::Mat
fsiv_create_gaussian_filter(const int r)
{
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO: Remenber 6*sigma is approx 99,73% of the distribution.
    int d       = 2*r+1;
    float sigma = pow(d/6,2);
    ret_v = cv::Mat::zeros(d,d,CV_32FC1); 

    for (int i = 0; i < d; ++i) 
        for (int j = 0; j < d; ++j) 
            ret_v.at<float>(i,j) = exp(-0.5*(pow(i-r, 2) + pow(j-r, 2))/sigma);

    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==(2*r+1) && ret_v.rows==ret_v.cols);
    CV_Assert(std::abs(1.0-cv::sum(ret_v)[0])<1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_create_laplacian_filter(const int filter_type, int r1, int r2)
{
    CV_Assert(0<=filter_type && filter_type<=2);
    CV_Assert(0<r1 && r1<r2);
    cv::Mat filter;
    //TODO
    //Rember DoG = G[r2]-G[r1].

    //
    CV_Assert(filter.type()==CV_32FC1);
    CV_Assert((filter_type == 2) || (filter.rows==3 && filter.cols==3) );
    CV_Assert((filter_type != 2) || (filter.rows==(2*r2+1) &&
                                     filter.cols==(2*r2+1)));
    return filter;
}

cv::Mat
fsiv_fill_expansion(cv::Mat const& in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO:
    //Hint you don't need use any for sentence.


    //
    CV_Assert(ret_v.type()==in.type());
    CV_Assert(ret_v.rows == in.rows+2*r);
    CV_Assert(ret_v.cols == in.cols+2*r);
    return ret_v;
}

cv::Mat
fsiv_circular_expansion(cv::Mat const& in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO
    //Hint you don't need use any for sentence.



    //
    CV_Assert(ret_v.type()==in.type());
    CV_Assert(ret_v.rows == in.rows+2*r);
    CV_Assert(ret_v.cols == in.cols+2*r);
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(0,0)==in.at<uchar>(in.rows-r, in.cols-r));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(0,ret_v.cols/2)==in.at<uchar>(in.rows-r, in.cols/2));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(0,ret_v.cols-1)==in.at<uchar>(in.rows-r, r-1));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows/2,0)==in.at<uchar>(in.rows/2, in.cols-r));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows/2,ret_v.cols/2)==in.at<uchar>(in.rows/2, in.cols/2));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows-1,0)==in.at<uchar>(r-1, in.cols-r));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows-1,ret_v.cols/2)==in.at<uchar>(r-1, in.cols/2));
    CV_Assert(!(in.type()==CV_8UC1) || ret_v.at<uchar>(ret_v.rows-1,ret_v.cols-1)==in.at<uchar>(r-1, r-1));
    return ret_v;
}

cv::Mat
fsiv_filter2D(cv::Mat const& in, cv::Mat const& filter)
{
    CV_Assert(!in.empty() && !filter.empty());
    CV_Assert(in.type()==CV_32FC1 && filter.type()==CV_32FC1);
    cv::Mat ret_v;
    //TODO


    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==in.rows-2*(filter.rows/2));
    CV_Assert(ret_v.cols==in.cols-2*(filter.cols/2));
    return ret_v;
}

cv::Mat
fsiv_extend_image(const cv::Mat& img, const cv::Size& new_size)
{
    CV_Assert(img.rows<new_size.height);
    CV_Assert(img.cols<new_size.width);
    cv::Mat out;
    //TODO


    //
    CV_Assert(out.type()==img.type());
    CV_Assert(out.rows == new_size.height);
    CV_Assert(out.cols == new_size.width);
    return out;
}

cv::Mat
fsiv_image_sharpening(const cv::Mat& in, float gain, int filter_type, bool only_luma,
                      int r1, int r2, bool circular)
{
    CV_Assert(in.depth()==CV_8U);
    CV_Assert(0<r1 && r1<r2);
    CV_Assert(0<=filter_type && filter_type<=2);
    cv::Mat out;
    //TODO



    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows);
    CV_Assert(out.cols==in.cols);
    return out;
}
