#include "common_code.hpp"

cv::Mat
fsiv_create_box_filter(const int r)
{
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO

    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==(2*r+1) && ret_v.rows==ret_v.cols);
    CV_Assert(std::abs(1.0-cv::sum(ret_v)[0])<1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_create_gaussian_filter(const int r)
{
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO: Remenber 6*sigma is approx 99,73% of the distribution.


    //
    CV_Assert(ret_v.type()==CV_32FC1);
    CV_Assert(ret_v.rows==(2*r+1) && ret_v.rows==ret_v.cols);
    CV_Assert(std::abs(1.0-cv::sum(ret_v)[0])<1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_fill_expansion(cv::Mat const& in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r>0);
    cv::Mat ret_v;
    //TODO:
    //Use of cv::copyMakeBorder is not allowed.
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
    //Use of cv::copyMakeBorder is not allowed.
    //Hint you don't need use any "for" sentence, only 9 copyTo from "in"
    // rois to "ret_v" rois.


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
fsiv_combine_images(const cv::Mat src1, const cv::Mat src2,
                    double a, double b)
{
    CV_Assert(src1.type()==src2.type());
    CV_Assert(src1.rows==src2.rows);
    CV_Assert(src1.cols==src2.cols);
    cv::Mat ret_v;
    //TODO


    //
    CV_Assert(ret_v.type()==src2.type());
    CV_Assert(ret_v.rows==src2.rows);
    CV_Assert(ret_v.cols==src2.cols);
    return ret_v;
}

cv::Mat
fsiv_usm_enhance(cv::Mat  const& in, double g, int r,
                 int filter_type, bool circular, cv::Mat *unsharp_mask)
{
    CV_Assert(!in.empty());
    CV_Assert(in.type()==CV_32FC1);
    CV_Assert(r>0);
    CV_Assert(filter_type>=0 && filter_type<=1);
    CV_Assert(g>=0.0);
    cv::Mat ret_v;
    //TODO
    //Hint: use your own functions fsiv_xxxx



    //
    CV_Assert(ret_v.rows==in.rows);
    CV_Assert(ret_v.cols==in.cols);
    CV_Assert(ret_v.type()==CV_32FC1);
    return ret_v;
}
