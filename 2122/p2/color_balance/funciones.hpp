#ifndef FUNCIONES_H

	//Includes para OpenCV, Descomentar según los módulo utilizados.
	#include <opencv2/core/core.hpp>
	#include <opencv2/core/utility.hpp>
	#include <opencv2/highgui/highgui.hpp>
	#include <opencv2/imgproc/imgproc.hpp>

	/**
	 * @brief Calcula el histograma de una imagen monocroma.
	 * @param in es la imagen con formato 8UC1.
	 * @param hist almacena el histograma calculado.
	 * @return el histograma calculado.
	 * @pre in.type()==CV_8UC1
	 * @pre hist.empty()||(hist.type()==CV_32FC1 && hist.rows==256 && hist.cols==1)
	 * @post hist.type()==CV_32FC1
	 * @post hist.rows==256 && hist.cols==1
	 */
	cv::Mat fsiv_compute_histogram(const cv::Mat& in, cv::Mat& hist);

	/**
	 * @brief Normaliza el histogram de forma que su suma sea 1.0
	 * @param hist el histograma a normalizar.
	 * @pre hist.type()==CV_32FC1
	 * @pre hist.rows==256 && hist.cols==1
	 * @post hist.type()==CV_32FC1
	 * @post hist.rows==256 && hist.cols==1
	 * @post sum(hist)[0]==0.0 || abs(sum(hist)[0]-1.0)<=1.0e-6
	 */
	void fsiv_normalize_histogram(cv::Mat& hist);

	/**
	 * @brief acumula el histograma.
	 * @param hist el histograma a acumular.
	 * @pre hist.type()==CV_32FC1
	 * @pre hist.rows==256 && hist.cols==1
	 * @post hist.type()==CV_32FC1
	 * @post hist.rows==256 && hist.cols==1
	 * @post hist[255]==0.0 || abs(hist[255]-1.0)<=1.0e-6
	 */
	void fsiv_accumulate_histogram(cv::Mat& hist);

	void calc_hist(const cv::Mat& in, cv::Mat& hist);

#define FUNCIONES_H
#endif
