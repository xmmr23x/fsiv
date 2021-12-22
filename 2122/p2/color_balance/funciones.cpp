#include "funciones.hpp"

cv::Mat fsiv_compute_histogram(const cv::Mat& in, cv::Mat& hist) {
	//TODO
	//Tienes dos alternativas:
	//1- Implementar un recorrido por la imagen y calcular el histograma.
	//2- Usar la función cv::calcHist.
	//Sugerencia: implementa las dos para comparar.
	int histSize        = 256;
	float range[]       = {0.0,256.0};
	const float *ranges = {range};
	
	cv::calcHist(&in, 1, 0, cv::Mat(), hist, 1, &histSize, &ranges);
	//

	return hist;
}

void fsiv_normalize_histogram(cv::Mat& hist) {
	//TODO
	cv::normalize(hist, hist, 1, 0, cv::NORM_L1);
}

void fsiv_accumulate_histogram(cv::Mat& hist) {
	//TODO
	for (int i = 1; i < hist.rows; ++i)
		hist.at<float>(i,0) += hist.at<float>(i-1,0);
}

void calc_hist(const cv::Mat& in, cv::Mat& hist) {
	fsiv_compute_histogram(in, hist);
	fsiv_normalize_histogram(hist);
	fsiv_accumulate_histogram(hist);
}
