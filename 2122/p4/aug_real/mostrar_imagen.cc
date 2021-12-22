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
	"{@file          |      | archivo xml   }"
	;

int
main (int argc, char* const* argv) {
	int retCode=EXIT_SUCCESS;

	try {    

		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Application name v1.0.0");
		
		if (parser.has("help")) {
			parser.printMessage();
			return 0;
		}
		
		cv::String filename = parser.get<cv::String>("@file");
		
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}

		cv::FileStorage fs(filename, cv::FileStorage::READ);
		if (!fs.isOpened()) return false;

		cv::Mat my_img, your_img;
		fs["my_img"] >> my_img;
		fs["your_img"] >> your_img;

		cv::namedWindow("my");
		cv::namedWindow("your");
		cv::imshow("my", my_img);
		cv::imshow("your", your_img);
		cv::waitKey();
	}
	
	catch (std::exception& e) {
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
return retCode;
}

