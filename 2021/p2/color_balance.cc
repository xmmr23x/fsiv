/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
	g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

const cv::String keys =
	"{help h usage ? |      | print this message   }"
	"{p              |0     | Percentage of brightest points used. Default 0 means use the most.}"
	"{@input         |<none>| input image.}"
	"{@output        |<none>| output image.}"
	;


/**
 * @brief Apply a "white patch" color balance operation to the image.
 * @arg[in] in is the imput image.
 * @return the color balanced image.
 * @pre in.type()==CV_8UC3
 * @warning A BGR color space is assumed for the input image.
 */
cv::Mat fsiv_wp_color_balance(cv::Mat const& in)
{
	CV_Assert(in.type()==CV_8UC3);

	//TODO
	cv::Mat img_gris;
	cv::cvtColor(in, img_gris, cv::COLOR_BGR2GRAY, 1);

	// minMaxLoc() busca el mayor y el menor valor de la imagen (0-255)
	cv::Point punto_max;
	cv::minMaxLoc(img_gris, NULL, NULL, NULL, &punto_max);

	// at<>() devuelve el valor de la imagen en un punto. 
	cv::Vec3b max = in.at<cv::Vec3b>(punto_max);

	//cv::split(array in, matriz out) divide la imagen en 3 canales distintos.
	cv::Mat in_bgr[3];
	cv::split(in, in_bgr);

	for (int i = 0; i < 3; i++) in_bgr[i] *= 255.0 / max[i];

	// cv::merge(matriz out, array in) combina los canales en una sola imagen.
	cv::Mat img_wp;
	merge(in_bgr, 3, img_wp);

	return img_wp;
}

cv::Scalar mediaHistogramaBGR(cv::Mat const &in, int p) {
	cv::Mat img_gris;
	cv::cvtColor(in, img_gris, cv::COLOR_BGR2GRAY, 1);
	float b = 0.0, g = 0.0, r = 0.0; 

	// porcentaje de puntos
	int n_puntos = cvRound(in.cols * in.rows*p/100);

	// minMaxLoc() busca el mayor y el menor valor de la imagen (0-255)
	for (int i = 0; i < n_puntos; ++i) {
		cv::Point punto_max;
		cv::minMaxLoc(img_gris, NULL, NULL, NULL, &punto_max);
		cv::Vec3b max = in.at<cv::Vec3b>(punto_max);
		b += max[0];
		g += max[1];
		r += max[2];
		img_gris.at<uchar>(punto_max.y, punto_max.x) = 0;
	}

	return cv::Scalar(b/n_puntos, g/n_puntos, r/n_puntos);
}

/**
 * @brief Apply a "gray world" color balance operation to the image.
 * @arg[in] in is the imput image.
 * @arg[in] p is the percentage of brightest points used to calculate the color correction factor.
 * @return the color balanced image.
 * @pre in.type()==CV_8UC3
 * @pre 0.0 < p <= 100.0
 * @warning A BGR color space is assumed for the input image.
 */
cv::Mat fsiv_gw_color_balance(cv::Mat const& in, float p)
{
	CV_Assert(in.type()==CV_8UC3);
	CV_Assert(0.0f<p && p<=100.0f);

	//TODO
	float n;
	cv::Scalar media;

	//cv::split(array in, matriz out) divide la imagen en 3 canales distintos.
	cv::Mat in_bgr[3];
	cv::split(in, in_bgr);

	if (p == 100) {
		// cv::mean() devuelve la media de cada canal de la imagen.
		media = cv::mean(in);
		n = 128.0;
	} else {
		n = 255.0;
		media = mediaHistogramaBGR(in, int(p));
	}

	for (int i = 0; i < 3; i++) in_bgr[i] *= n / media[i];

	// cv::merge(matriz out, array in) combina los canales en una sola imagen.
	cv::Mat img_wp;
	merge(in_bgr, 3, img_wp);


	return cv::Mat(img_wp);
}

/**
 * @brief Application State.
 * Use this structure to maintain the state of the application
 * that will be passed to the callbacks.
 */
struct UserData
{
	//TODO

};

/** @brief Standard mouse callback
 * Use this function an argument for cv::setMouseCallback to control the
 * mouse interaction with a window.
 *
 * @arg event says which mouse event (move, push/release a mouse button ...)
 * @arg x and
 * @arg y says where the mouse is.
 * @arg flags give some keyboard state.
 * @arg user_data allow to pass user data to the callback.
 */
void on_mouse (int event, int x, int y, int flags, void * user_data_)
{
	UserData *user_data = static_cast<UserData*>(user_data_);
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		//TODO

	}
}

/** @brief Standard trackbar callback
 * Use this function an argument for cv::createTrackbar to control
 * the trackbar changes.
 *
 * @arg v give the trackbar position.
 * @arg user_data allow to pass user data to the callback.
 */
void on_change(int v, void * user_data_)
{
	UserData * user_data = static_cast<UserData*>(user_data_);

	//TODO
}

int
main (int argc, char* const* argv)
{
	int retCode=EXIT_SUCCESS;

	try {

		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Apply a color balance to an image.");
		if (parser.has("help"))
		{
			parser.printMessage();
			return EXIT_SUCCESS;
		}
		int p = parser.get<int>("p");
		if (p<0 || p>100)
		{
			std::cerr << "Error: p is out of range [0, 100]." << std::endl;
			return EXIT_FAILURE;
		}
		cv::String input_n = parser.get<cv::String>("@input");
		cv::String output_n = parser.get<cv::String>("@output");
		if (!parser.check())
		{
			parser.printErrors();
			return EXIT_FAILURE;
		}

		//TODO
		cv::Mat img_in, img_out;
		img_in = cv::imread(input_n, cv::IMREAD_UNCHANGED);
		
		cv::namedWindow("IMG");
		cv::imshow("IMG", img_in);

		if (parser.has("p") && p > 0) img_out = fsiv_gw_color_balance(img_in, p);
		else img_out = fsiv_wp_color_balance(img_in);

		cv::namedWindow("IMG2");
		cv::imshow("IMG2", img_out);

		while (true) {
			int key = cv::waitKey(0);
			if (key == 27) break;
			else {
				cv::imwrite(output_n, img_out);
				break;
			}
		}


//		if (cv::waitKey(0) != 27)
//			cv::imwrite(img2, img_out);
	}
	catch (std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
	return retCode;
}
