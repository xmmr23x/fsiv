#include "common_code.hpp"
#include "funciones.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cv::Mat fsiv_color_rescaling(const cv::Mat& in, const cv::Scalar& from, const cv::Scalar& to)
{
	CV_Assert(in.type()==CV_8UC3);
	cv::Mat out;

	//TODO
	//Cuidado con dividir por cero.
	//Evita los bucles.
	cv::Scalar prueba;

	if (from[0] == 0 || from[1] == 0 || from[2] == 0) out = in.clone();
	else
		prueba = cv::Scalar(to[0]/from[0],to[1]/from[1],to[2]/from[2]);

	out = in.mul(prueba);
	//

	CV_Assert(out.type()==in.type());
	CV_Assert(out.rows==in.rows && out.cols==in.cols);
	return out;
}

cv::Mat fsiv_wp_color_balance(cv::Mat const& in)
{
	CV_Assert(in.type()==CV_8UC3);
	cv::Mat out;

	//TODO
	//Sugerencia: utiliza el espacio de color GRAY para
	//saber la ilumimancia de un pixel.
	cv::Mat img_gris;
	cv::cvtColor(in, img_gris, cv::COLOR_BGR2GRAY, 1);

	// minMaxLoc() busca el mayor y el menor valor de la imagen (0-255)
	cv::Point punto_max;
	cv::minMaxLoc(img_gris, NULL, NULL, NULL, &punto_max);
	cv::Vec3b maxi = in.at<cv::Vec3b>(punto_max);

	cv::Scalar white(255.0,255.0,255.0);
	cv::Scalar max(maxi[0], maxi[1], maxi[2]);

	out = fsiv_color_rescaling(in, max, white);
	//

	CV_Assert(out.type()==in.type());
	CV_Assert(out.rows==in.rows && out.cols==in.cols);
	return out;
}

cv::Mat fsiv_gw_color_balance(cv::Mat const& in)
{
	CV_Assert(in.type()==CV_8UC3);
	cv::Mat out;

	//TODO
	cv::Scalar media = cv::mean(in);
	cv::Scalar gray(128.0,128.0,128.0);

	out = fsiv_color_rescaling(in, media, gray);
	//

	CV_Assert(out.type()==in.type());
	CV_Assert(out.rows==in.rows && out.cols==in.cols);
	return out;
}

cv::Mat fsiv_color_balance(cv::Mat const& in, float p)
{
	CV_Assert(in.type()==CV_8UC3);
	CV_Assert(0.0f<p && p<100.0f);
	cv::Mat out;
	//TODO
	//Sugerencia: utiliza el espacio de color GRAY para
	//saber la ilumimancia de un pixel.
	cv::Mat img_gris;
	cv::cvtColor(in, img_gris, cv::COLOR_BGR2GRAY, 1);

	cv::Mat hist;
	float t, n; 
	p /= 100.0;

	calc_hist(img_gris, hist);

	for (int i = 0; i < 256; ++i) {
		n = hist.at<float>(i,0);
		if (n >= p) {
			t = n;
			break;
		}
	}

	cv::Mat aux(img_gris>=t);
	cv::Scalar white(255.0,255.0,255.0);
	cv::Scalar media = cv::mean(in, aux);

	out = fsiv_color_rescaling(in, media, white);
	//

	CV_Assert(out.type()==in.type());
	CV_Assert(out.rows==in.rows && out.cols==in.cols);
	return out;
}
