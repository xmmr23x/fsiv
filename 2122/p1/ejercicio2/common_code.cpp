#include "common_code.hpp"
#include <iostream>

cv::Mat
convert_image_byte_to_float(const cv::Mat& img, cv::Mat& out)
{
	CV_Assert(img.depth()==CV_8U);
	
	//TODO
	img.convertTo(out, CV_32F, 1.0/255.0);
	//
	
	CV_Assert(out.rows==img.rows && out.cols==img.cols);
	CV_Assert(out.depth()==CV_32F);
	CV_Assert(img.channels()==out.channels());
	return out;
}

cv::Mat
convert_image_float_to_byte(const cv::Mat& img, cv::Mat& out)
{
	CV_Assert(img.depth()==CV_32F);
	
	//TODO
	img.convertTo(out, CV_8U, 255.0);
	//
	
	CV_Assert(out.rows==img.rows && out.cols==img.cols);
	CV_Assert(out.depth()==CV_8U);
	CV_Assert(img.channels()==out.channels());
	return out;
}

cv::Mat
convert_bgr_to_hsv(const cv::Mat& img, cv::Mat& out)
{
	CV_Assert(img.channels()==3);
	//TODO
	cv::cvtColor(img, out, cv::COLOR_BGR2HSV);
	//
	CV_Assert(out.channels()==3);
	return out;
}

cv::Mat
convert_hsv_to_bgr(const cv::Mat& img, cv::Mat& out)
{
	CV_Assert(img.channels()==3);
	//TODO
	cv::cvtColor(img, out, cv::COLOR_HSV2BGR);
	//
	CV_Assert(out.channels()==3);
	return out;
}

cv::Mat
cbg_process (const cv::Mat & in, cv::Mat& out,
			 double contrast, double brightness, double gamma,
			 bool only_luma)
{
	CV_Assert(in.depth()==CV_8U);
	//TODO
	//Recuerda: es recomendable trabajar en flotante [0,1]
	//Después deshacer el cambio a byte [0,255]
	cv::Mat aux_in = in.clone(), aux_out, hsv[3];

	if (only_luma && in.channels() == 3) {
		aux_in = convert_bgr_to_hsv(aux_in, aux_in);
		cv::split(aux_in, hsv);
		aux_in = hsv[2].clone();
	}
	
	aux_in = convert_image_byte_to_float(aux_in, aux_in);

	cv::pow(aux_in, gamma, aux_in);
	cv::Scalar c(contrast, contrast, contrast), b(brightness, brightness, brightness);
	aux_out = aux_in.mul(c) + b;

	out = convert_image_float_to_byte(aux_out, aux_out);

	if (only_luma && in.channels() == 3) {
		hsv[2] = out.clone();
		cv::merge(hsv, 3, out);
		out = convert_hsv_to_bgr(out, out);
	}
	//
	CV_Assert(out.rows==in.rows && out.cols==in.cols);
	CV_Assert(out.depth()==CV_8U);
	CV_Assert(out.channels()==in.channels());
	return out;
}
