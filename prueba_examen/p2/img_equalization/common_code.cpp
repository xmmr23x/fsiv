#include <iostream>
#include "common_code.hpp"

cv::Mat
fsiv_compute_histogram(const cv::Mat& in, cv::Mat& hist)
{
	CV_Assert(in.type()==CV_8UC1);
	CV_Assert(hist.empty() ||(hist.type()==CV_32FC1 &&
							  hist.rows==256 && hist.cols==1));
	//TODO
	//Tienes dos alternativas:
	//1- Implementar un recorrido por la imagen y calcular el histograma.
	//2- Usar la función cv::calcHist.
	//Sugerencia: implementa las dos para comparar.
	int histSize = 256;
	float range[] = {0.0, 256.0};
	const float *ranges = {range};

	cv::calcHist(&in, 1, 0, cv::Mat(), hist, 1, &histSize, &ranges);
	//

	CV_Assert(hist.type()==CV_32FC1);
	CV_Assert(hist.rows==256 && hist.cols==1);
	return hist;
}

void
fsiv_normalize_histogram(cv::Mat& hist)
{
	CV_Assert(hist.type()==CV_32FC1);
	CV_Assert(hist.rows==256 && hist.cols==1);

	//TODO
	cv::normalize(hist, hist, 1, 0, cv::NORM_L1);

	//
	CV_Assert(hist.type()==CV_32FC1);
	CV_Assert(hist.rows==256 && hist.cols==1);
}

void
fsiv_accumulate_histogram(cv::Mat& hist)
{
	CV_Assert(hist.type()==CV_32FC1);
	CV_Assert(hist.rows==256 && hist.cols==1);

	//TODO
	for (int i = 1; i < hist.rows; ++i)
		hist.at<float>(i,0) += hist.at<float>(i-1,0);
	//
}

cv::Mat
fsiv_create_equalization_lookup_table(const cv::Mat& hist,
									  bool hold_median)
{
	CV_Assert(hist.type()==CV_32FC1);
	CV_Assert(hist.rows==256 && hist.cols==1);
	cv::Mat lkt;
	//TODO
	//Usa las funciones fsiv_normalize_histogram y fsiv_accumulate_histogram
	//para construir la tabla.
	cv::Mat hist1 = cv::Mat::zeros(hist.rows, hist.cols, hist.type());
	cv::Mat hist2 = cv::Mat::zeros(hist.rows, hist.cols, hist.type());
	lkt = hist.clone();
	int m;
	
	fsiv_normalize_histogram(lkt);
	fsiv_accumulate_histogram(lkt);

	lkt.convertTo(lkt, CV_8UC1, 255.0);
	//

	CV_Assert(lkt.type()==CV_8UC1);
	CV_Assert(lkt.rows==256 && lkt.cols==1);
	return lkt;
}

cv::Mat
fsiv_apply_lookup_table(const cv::Mat&in, const cv::Mat& lkt,
						cv::Mat& out)
{
	CV_Assert(in.type()==CV_8UC1);
	CV_Assert(lkt.type()==CV_8UC1);
	CV_Assert(lkt.rows==256 && lkt.cols==1);
	CV_Assert(out.empty() || (out.type()==CV_8UC1 &&
							  out.rows==in.rows && out.cols==in.cols));

	//TODO
	out = in.clone();

	for (int r = 0; r < in.rows; ++r)
		for (int c = 0; c < in.rows; ++c)
			out.at<unsigned char>(r,c) = lkt.at<unsigned char>(in.at<unsigned char>(r,c),0);
	//

	CV_Assert(out.rows ==in.rows && out.cols==in.cols && out.type()==in.type());
	return out;
}

cv::Mat
fsiv_image_equalization(const cv::Mat& in, cv::Mat& out,
					bool hold_median, int radius)
{
	CV_Assert(in.type()==CV_8UC1);
	//TODO
	//Utiliza las funciones fsiv_compute_histogram,
	//fsiv_create_equalization_lookup_table y fsiv_apply_lookup_table
	//
	cv::Mat hist, lkt;

	hist = fsiv_compute_histogram(in, hist);
	lkt  = fsiv_create_equalization_lookup_table(hist, hold_median);
	out  = fsiv_apply_lookup_table(in, lkt, out);

	//
	CV_Assert(out.rows==in.rows && out.cols==in.cols && out.type()==in.type());
	return out;
}
