#include "common_code.hpp"
#include <iostream>

cv::Mat
fsiv_create_box_filter(const int r)
{
	CV_Assert(r>0);
	cv::Mat ret_v;
	//TODO
	int size = 2*r+1;
	ret_v = cv::Mat(size, size, CV_32FC1, cv::Scalar(1.0/(size*size)));
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
	int size    = 2*r+1;
	float sigma = pow(size/6.0, 2);
	float c     = 1.0/(2*M_PI*sigma);

	ret_v = cv::Mat(size, size, CV_32FC1, cv::Scalar(1.0));

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int n                 = -(pow(i-r, 2) + pow(j-r, 2));
			ret_v.at<float>(i, j) = c * exp(n/(2*sigma));
		}
	}

	cv::Scalar sum = cv::sum(ret_v);
	ret_v         /= sum;

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
	//Hint you don't need use any for sentence.
	ret_v = cv::Mat(in.rows + 2*r, in.cols + 2*r, in.type(), cv::Scalar(0));
	in.copyTo(ret_v(cv::Rect(r, r, in.cols, in.rows)));
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
	ret_v = fsiv_fill_expansion(in, r);

	// expansion superior e inferior
	in(cv::Rect(0,0,in.cols,r)).copyTo(ret_v(cv::Rect(r, ret_v.rows-r, in.cols,r)));
	in(cv::Rect(0,in.rows-r,in.cols,r)).copyTo(ret_v(cv::Rect(r, 0, in.cols,r)));

	// expansion lateral
	in(cv::Rect(0,0,r,in.rows)).copyTo(ret_v(cv::Rect(ret_v.cols-r,r,r,in.rows)));
	in(cv::Rect(in.cols-r,0,r,in.rows)).copyTo(ret_v(cv::Rect(0,r,r,in.rows)));

	// expansion en las esquinas
	in(cv::Rect(0,0,r,r)).copyTo(ret_v(cv::Rect(ret_v.cols-r,ret_v.rows-r,r,r)));
	in(cv::Rect(in.cols-r,in.rows-r,r,r)).copyTo(ret_v(cv::Rect(0,0,r,r)));
	in(cv::Rect(in.cols-r,0,r,r)).copyTo(ret_v(cv::Rect(0,ret_v.rows-r,r,r)));
	in(cv::Rect(0,in.rows-r,r,r)).copyTo(ret_v(cv::Rect(ret_v.cols-r,0,r,r)));
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
	int r = in.rows-2*(filter.rows/2);
	int c = in.cols-2*(filter.cols/2);
	ret_v = cv::Mat::zeros(r, c, CV_32FC1);

	for (int i = 0; i < ret_v.rows; ++i) {
		for (int j = 0; j < ret_v.cols; ++j)
			ret_v.at<float>(i,j) = sum(filter.mul(in(cv::Rect(j, i, filter.cols, filter.rows)))).val[0];
	}
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
	ret_v = src1.mul(a) + src2.mul(b);
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


	cv::Mat f, expand;
	if (filter_type == 0) f = fsiv_create_box_filter(r);
	else f = fsiv_create_gaussian_filter(r);
	if (!circular) expand = fsiv_fill_expansion(in, r);
	else expand = fsiv_circular_expansion(in, r);

	cv::Mat conv = fsiv_filter2D(expand, f);
	ret_v        = (g+1) * in - (g * conv);

	if (unsharp_mask) *unsharp_mask = conv;

	//
	CV_Assert(ret_v.rows==in.rows);
	CV_Assert(ret_v.cols==in.cols);
	CV_Assert(ret_v.type()==CV_32FC1);
	return ret_v;
}
