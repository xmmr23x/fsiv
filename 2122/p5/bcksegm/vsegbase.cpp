/* 
   (c) Fundamentos de Sistemas Inteligenties en Vision
   University of Cordoba, Spain
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
//#include <unistd.h>
#include <ctype.h>
#include <cmath>


#include <opencv2/core/utility.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

#include "common_code.hpp"


/*
  Use CMake to compile
*/

const cv::String keys =
		"{help h usage ? |      | print this message   }"
		"{t threshold    |13    | Segmentation threshold.}"
		"{s              |0     | Radius of structural element. 0 means not remove.}"
		"{g              |0     | Radius of the gaussian kernel. 0 means not average.}"
		"{c              |      | Use this camera idx.}"
		"{v              |      | Use this video file.}"
		"{@output        |<none>| Path to output video.}"
		;

int
main (int argc, char * const argv[])
{

	cv::CommandLineParser parser(argc, argv, keys);
	parser.about("Background segmentation in video.");
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}
	cv::VideoCapture input;
	bool is_camera = false;
	if (parser.has("c"))
	{
		input.open(parser.get<int>("c"));
		is_camera=true;
	}
	else if (parser.has("v"))
	{
		input.open(parser.get<std::string>("v"));
	}
	else
	{
		std::cerr << "Error: Wrong CLI. one of 'v' or 'c' options must be specified."
				  << std::endl;
		return EXIT_FAILURE;
	}
	if (! input.isOpened())
	{
		std::cerr << "Error: could not open the input video stream."
				  << std::endl;
		return EXIT_FAILURE;
	}

	cv::Mat prev_frame;
	bool was_Ok = input.read(prev_frame);
	if (!was_Ok)
	{
		std::cerr << "Error: could not read any image from the input stream.\n";
		return EXIT_FAILURE;
	}

	std::string output_path = parser.get<std::string>("@output");
	int threshold = parser.get<int>("threshold");
	int ste_radius = parser.get<int>("s");
	int g_radius = parser.get<int>("g");

	cv::VideoWriter output;
	double fps=25.0; //Default value for live video.
	//TODO
	//Open the output video stream.
	//If the input is a video file, get the fps from it.
	if (parser.has("v"))
		fps = input.get(cv::CAP_PROP_FPS);

	output.open(
		output_path, 
		cv::VideoWriter::fourcc('M','J','P','G'),
		fps,
		prev_frame.size()
	);
	//

	if (!output.isOpened())
	{
		std::cerr << "Error: could not open the output stream ["
				  << output_path << "]." << std::endl;
		return EXIT_FAILURE;
	}

	cv::namedWindow("Input");
	cv::namedWindow("Masked Input");
	cv::namedWindow("Output");
	//TODO
	//Trackbars to window Output to control the parameters.



	//

	cv::Mat curr_frame, mask, masked_frame;
	int key = 0;
	curr_frame = prev_frame.clone();
	
	while(was_Ok && key!=27)
	{
		//TODO
		//Process the input stream until achiving the last frame or press "ESC" key.
		//First get the curr frame and used the prev one to compute the mask.
		//Try adjust waitKey's time to approximate the fps of the stream.

		if (g_radius > 0) {
			cv::Size s(2*g_radius+1,2*g_radius+1);
			cv::GaussianBlur(prev_frame, prev_frame, s, 0);
			cv::GaussianBlur(curr_frame, curr_frame, s, 0);
		}

		fsiv_segm_by_dif(prev_frame, curr_frame, mask, threshold, ste_radius);
		fsiv_apply_mask(curr_frame, mask, masked_frame);

		cv::imshow("Input", curr_frame);
		cv::imshow("Masked Input", masked_frame);
		cv::imshow("Output", mask);

		output.write(mask);

		prev_frame   = curr_frame.clone();
		was_Ok       = input.read(curr_frame);
		masked_frame = cv::Mat();

		key = cv::waitKey(20) & 0xff;
		//
	}

	cv::destroyAllWindows();


	return EXIT_SUCCESS;
}
