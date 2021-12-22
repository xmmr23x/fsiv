/**
 * @author Manuel Mariscal
 * @date 29/09/2020
 * @file funciones.cc
 * @brief Implementación de las funciones.
 * @version 1.0
 */

#include "funciones.h"

void onMouse(int event, int x, int y, int flags, void *imagenes) {
	cv::Mat img_loc = static_cast<cv::Mat *> (imagenes)[0].clone();
	static bool punto_a_ok = false, punto_b_ok = false;
	static bool levanta_boton = false, mueve_raton = false;
	static cv::Point punto_a, punto_b;

	if (punto_a_ok  && !punto_b_ok) {
		if (event == cv::EVENT_MOUSEMOVE) {
			cv::rectangle(img_loc, punto_a, cv::Point(x, y), cv::Scalar(5,252,116));
			cv::imshow("IMG", img_loc);
		}

		if (event == cv::EVENT_MOUSEMOVE && !levanta_boton) mueve_raton = true;
		else if (event == cv::EVENT_LBUTTONUP && !mueve_raton) levanta_boton = true;
		else if ((event == cv::EVENT_LBUTTONUP && mueve_raton) ||
			(event == cv::EVENT_LBUTTONDOWN && levanta_boton)) {
			punto_b_ok = true;
			punto_b.x = x;
			punto_b.y = y;
			cv::Rect plano_ab(punto_a, punto_b);
			cv::rectangle(img_loc, plano_ab, cv::Scalar(5,252,116));

			// generamos la mascara
			cv::Mat mask(img_loc.rows, img_loc.cols, CV_8U, cv::Scalar::all(0));
			cv::rectangle(mask, plano_ab, cv::Scalar::all(255), CV_FILLED, cv::LINE_AA);

			setGrises(mask, static_cast<cv::Mat *> (imagenes));
		}
	} else if (event == cv::EVENT_LBUTTONDOWN) {
		punto_a_ok = true;
		punto_a.x = x;
		punto_a.y = y;
	}
}

void onMousePoligono(int event, int x, int y, int flags, void *imagenes) {
	static cv::Mat img_estat = static_cast<cv::Mat *> (imagenes)[0].clone();
	cv::Mat img_loc = img_estat.clone();
	static std::vector<cv::Point> puntos;
	static bool ok = false;

	if (!ok) {
		if (event == cv::EVENT_MOUSEMOVE && !puntos.empty()) {
			cv::line(img_loc, puntos.back(), cv::Point(x,y), cv::Scalar(5,252,116));
			cv::imshow("IMG", img_loc);
		}

		if (event == cv::EVENT_LBUTTONDOWN) {
			if (!puntos.empty()) 
				cv::line(img_estat, puntos.back(), cv::Point(x,y), cv::Scalar(5,252,116));

			puntos.push_back(cv::Point(x,y));
		}

		if (event == cv::EVENT_RBUTTONUP && puntos.size() > 2) {
			// generamos la mascara
			cv::Mat mask(img_loc.rows, img_loc.cols, CV_8U, cv::Scalar::all(0));
			cv::fillPoly(mask, puntos, cv::Scalar::all(255), cv::LINE_4);

			setGrises(mask, static_cast<cv::Mat *> (imagenes));
			ok = true;
		}
	}
}

void setGrises(cv::Mat mask, cv::Mat *imagenes) {
	cv::Mat color = imagenes[0].clone();

	// generamos la imagen en gris
	cv::Mat gris;
	cv::cvtColor(color, gris, cv::COLOR_BGR2GRAY);
	cv::cvtColor(gris, gris, cv::COLOR_GRAY2BGR);
	
	// combinamos las imagenes a traves de la mascara
	color.copyTo(gris, mask);	

	cv::imshow("IMG", gris);
	imagenes[1] = gris.clone();
}