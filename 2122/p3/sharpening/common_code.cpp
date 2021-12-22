#include <iostream>
#include "common_code.hpp"
#include <opencv2/imgproc.hpp>

cv::Mat
fsiv_create_gaussian_filter(const int r)
{
	CV_Assert(r>0);
	cv::Mat ret_v;
	//TODO: Remenber 6*sigma is approx 99,73% of the distribution.
	int size    = 2*r+1;
	float sigma = pow(size/6.0, 2);

	ret_v = cv::Mat::zeros(size, size, CV_32FC1);

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			float n               = -0.5*(pow(i-r, 2) + pow(j-r, 2));
			ret_v.at<float>(i, j) = exp(n/(sigma));
		}
	}

	ret_v /= cv::sum(ret_v);

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
	filter = cv::Mat::ones(3,3,CV_32FC1);
	
	if (filter_type == 0) {
		filter.at<float>(0,0) = 0;
		filter.at<float>(0,2) = 0;
		filter.at<float>(2,0) = 0;
		filter.at<float>(2,2) = 0;
		filter.at<float>(1,1) = -4;
	} 
	else if (filter_type == 1) filter.at<float>(1,1) = -8;
	else {
		cv::Mat a, b;
		a = fsiv_create_gaussian_filter(r2);
		b = fsiv_create_gaussian_filter(r1);
		b = fsiv_extend_image(b, a.size());
		filter = a - b;
	}
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

	for (int i = 0; i < r; ++i)
		for (int j = 0; j < c; ++j)
			ret_v.at<float>(i,j) = sum(filter.mul(in(cv::Rect(j, i, filter.cols, filter.rows)))).val[0];

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
	int w = new_size.width, h = new_size.height;
	int r = img.rows, c = img.cols;
	
	out = cv::Mat::zeros(h, w, img.type());
	img.copyTo(out(cv::Rect((w-c)/2, (h-r)/2, c, r)));

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
	cv::Mat f, expand, hsv[3], aux_in = in.clone();
	int r; 

	// creacion del filtro
	if (filter_type == 2) r = r2;
	else r = 1;
	f = fsiv_create_laplacian_filter(filter_type, r1, r2);
	f = -f;
	f.at<float>(r,r) += gain;
	
	// procesamiento de una imagen a color en el canal luma
	if (only_luma) {
		cv::cvtColor(aux_in, aux_in, cv::COLOR_BGR2HSV);
		cv::split(aux_in, hsv);
		aux_in = hsv[2].clone();
	}

	// expansion de la imagen
	if (!circular) expand = fsiv_fill_expansion(aux_in, r);
	else expand = fsiv_circular_expansion(aux_in, r);
	
	expand.convertTo(expand,CV_32FC1);

	out = fsiv_filter2D(expand, f);

	out.convertTo(out,in.type());

	// procesamiento de una imagen a color en el canal luma
	if (only_luma) {
		hsv[2] = out.clone();
		cv::merge(hsv, 3, out);
		cv::cvtColor(out, out, cv::COLOR_HSV2BGR);
	}

	//
	CV_Assert(out.type()==in.type());
	CV_Assert(out.rows==in.rows);
	CV_Assert(out.cols==in.cols);
	return out;
}
