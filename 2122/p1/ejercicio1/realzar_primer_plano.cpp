/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
	g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>
#include <sstream>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const cv::String keys =
	"{help h usage ? |      | Print this message}"
	"{r              |      | Use a rectangle (x,y, widht, height)}"
	"{c              |      | Use a circle (x,y,radius)}"
	"{p              |      | Use a closed polygon x1,y1,x2,y2,x3,y3,...}"
	"{i              |      | Interactive mode.}"
	"{@input         | <none> | input image.}"
	"{@output        | <none> | output image.}"
	;

int
main (int argc, char* const* argv)
{
	int retCode=EXIT_SUCCESS;

	try {
		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Do a foreground enhance using a ROI.");
		if (parser.has("help"))
		{
			parser.printMessage();
			return EXIT_SUCCESS;
		}

		cv::String input_n = parser.get<cv::String>("@input");
		cv::String output_n = parser.get<cv::String>("@output");
		if (!parser.check())
		{
			parser.printErrors();
			return EXIT_FAILURE;
		}

		cv::Mat in = cv::imread(input_n, cv::IMREAD_UNCHANGED);
		if (in.empty())
		{
			std::cerr << "Error: could not open input image '" << input_n
					  << "'." << std::endl;
			return EXIT_FAILURE;
		}
		cv::Mat mask = in.clone();
		cv::Mat out = in.clone();

		//TODO

		//mask = generate_circle_mask(in.cols, in.rows, 250, 200, 100, in.type());
		out = convert_rgb_to_gray(out);
		out = convert_gray_to_rgb(out);

		if (parser.has("c")) {
			int x, y, r;
			char sep;
			std::istringstream input(parser.get<cv::String>("c"));
			input >> x >> sep >> y >> sep >> r;

			mask = generate_circle_mask(in.cols, in.rows, x, y, r, in.type());
		}

		else if (parser.has("r")) {
			int x, y, w, h;
			char sep;
			std::istringstream input(parser.get<cv::String>("r"));
			input >> x >> sep >> y >> sep >> w >> sep >> h;

			mask = generate_rectagle_mask(in.cols, in.rows, x, y, w, h, in.type());
		}

		else if (parser.has("p")) {
			std::vector<cv::Point> points;
			int x, y;
			char sep;
			std::istringstream input(parser.get<cv::String>("p"));

			do {
				input >> x >> sep >> y >> sep;
				points.push_back(cv::Point(x, y));
			} while(input);

			if (points.size() > 2)
				mask = generate_polygon_mask(in.cols, in.rows, points, in.type());
			else
				std::cout << "Error: Se necesitan al menos 3 puntos." << std::endl;
		}

		out = combine_images(in, out, mask);

		//
		cv::namedWindow("INPUT");
		cv::imshow("INPUT", in);
		cv::namedWindow("MASK");
		cv::imshow("MASK", mask);
		cv::namedWindow("OUTPUT");
		cv::imshow("OUTPUT", out);

		int k = cv::waitKey(0)&0xff;
		if (k!=27)
			cv::imwrite(output_n, out);
	}
	catch (std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
	return retCode;
}
