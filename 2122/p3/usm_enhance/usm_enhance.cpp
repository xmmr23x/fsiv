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

#include "common_code.hpp"

const cv::String keys =
	"{help h usage ? |      | print this message.}"
	"{i interactive  |      | Interactive mode.}"
	"{r radius       |1     | Window's radius. Default 1.}"
	"{g gain         |1.0   | Enhance's gain. Default 1.0}"
	"{c circular     |      | Use circular convolution.}"
	"{f filter       |0     | Filter type: 0->Box, 1->Gaussian. Default 0.}"
	"{@input         |<none>| input image.}"
	"{@output        |<none>| output image.}"
	;


int
main (int argc, char* const* argv)
{
	int retCode=EXIT_SUCCESS;

	try {
		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Apply an unsharp mask enhance to an image.");
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

		//TODO
		double g = parser.get<double>("g");
		int r = parser.get<int>("r");
		int filter_type = parser.get<int>("f");
		bool circular = parser.has("c");
		//

		cv::Mat in = cv::imread(input_n, cv::IMREAD_UNCHANGED);
		cv::Mat out = in.clone();
		cv::Mat mask = in.clone();

		if (in.empty())
		{
			std::cerr << "Error: could not open input image '" << input_n
					  << "'." << std::endl;
			return EXIT_FAILURE;
		}

		cv::namedWindow("INPUT");
		cv::namedWindow("OUTPUT");
		cv::namedWindow("UNSHARP MASK");

		cv::Mat aux_in = in.clone(), hsv[3];

		//TODO
		if (in.channels() == 3) {
			cv::cvtColor(aux_in, aux_in, cv::COLOR_BGR2HSV);
			cv::split(aux_in, hsv);
			aux_in = hsv[2].clone();
		}

		aux_in.convertTo(aux_in,CV_32FC1,1.0/255.0);

		out = fsiv_usm_enhance(aux_in, g, r, filter_type, circular, &mask);

		out.convertTo(out,in.type(),255.0);

		if (in.channels() == 3) {
			hsv[2] = out.clone();
			cv::merge(hsv, 3, out);
			cv::cvtColor(out, out, cv::COLOR_HSV2BGR);
		}

		cv::imshow ("INPUT", in);
		cv::imshow ("OUTPUT", out);
		cv::imshow ("UNSHARP MASK", mask);

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
