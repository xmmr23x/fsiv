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
	"{help h usage ? |      | Print this message   }"
	"{b bframes      |100   | Number of frames to estimate background model.}"
	"{a alpha        |0.01  | Weight for updating the background model.}"
	"{k threshold    |0.05  | Segmentation threshold.}"
	"{u stup         |10    | Short time updating period. Value 0 means not update.}"
	"{U ltup         |100   | Long time updating period. Value 0 means not update.}"
	"{r              |      | Radius to remove segmentation noise. Value 0 means not remove.}"
	"{g              |      | Radius to do a previous Gaussian averaging of input frames. Value 0 means not average.}"
	"{c              |      | Use this camera idx.}"
	"{v              |      | Use this video file.}"
	"{@output        |<none>| Path to output video.}"
	;

void
on_change_K(int v, void *user_data)
{
	std::cout << "Setting K= " << v/100.0 << std::endl;
}

void
on_change_alfa(int v, void *user_data)
{
	std::cout << "Setting Alfa= " << v/100.0 << std::endl;
}

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

	cv::Mat frame;
	bool was_Ok = input.read(frame);
	if (!was_Ok)
	{
		std::cerr << "Error: could not read any image from the input stream.\n";
		return EXIT_FAILURE;
	}

	std::string output_path = parser.get<std::string>("@output");
	float K = parser.get<float>("k");
	int K_int = int(K*100.0f);
	int ste_radius = parser.get<int>("r");
	int gauss_radius = parser.get<int>("g");
	int stup = parser.get<int>("u");
	int ltup = parser.get<int>("U");
	int num_frames = parser.get<int>("b");
	float alfa = parser.get<float>("a");
	int alfa_int = alfa*100.0f;

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
		frame.size()
	);


	//

	if (!output.isOpened())
	{
		std::cerr << "Error: could not open the output stream ["
				  << output_path << "]." << std::endl;
		return EXIT_FAILURE;
	}

	cv::namedWindow("Input"); //show the current input frame.
	cv::namedWindow("Masked Input"); //show the current frame masked
	cv::namedWindow("Output"); //show the segmentation (mask).
	cv::namedWindow("Background"); //show the model (mean image.)
	//TODO
	//Add trackbars to window Output to control the parameters.


	//

	cv::Mat Mean, Variance;
	//TODO
	//First, to learn the gaussian model.
	fsiv_learn_gaussian_model(input, Mean, Variance, num_frames, gauss_radius);
	cv::imshow("Background", Mean);
	//

	cv::Mat frame_f;
	cv::Mat mask;
	cv::Mat masked_frame;
	int key = 0;
	int frame_count = num_frames;
	while(was_Ok && key!=27)
	{
		//TODO
		//Process the input stream until achiving the last frame or press "ESC" key.
		//Remember: convert captured frames to in float [0,1].
		//First get the curr frame and use the learned model to segment it (get the mask).
		//Second update the model using the frame count.
		//Third get the masked frame.
		//Fourth show the results.
		//Fifth write the mask into the output RGB stream.
		//Try adjust waitKey's time to approximate the fps of the stream.
		frame.convertTo(frame_f, CV_32F, 1.0/255.0);

		if (gauss_radius > 0)
			cv::GaussianBlur(frame_f, frame_f, cv::Size(2*gauss_radius+1,2*gauss_radius+1), 0);


		fsiv_segm_by_gaussian_model(frame_f, mask, Mean, Variance, K, ste_radius);
		fsiv_update_gaussian_model(frame_f, mask, frame_count, Mean, Variance, alfa, stup, ltup);
		fsiv_apply_mask(frame, mask, masked_frame);

		cv::imshow("Input", frame_f);
		cv::imshow("Masked Input", masked_frame);
		cv::imshow("Output", mask);

		output.write(mask);

		was_Ok       = input.read(frame);
		masked_frame = cv::Mat();
		frame_count++;

		key = cv::waitKey(fps) & 0xff;
		//
	}

	return EXIT_SUCCESS;
}
