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
#include <opencv2/calib3d/calib3d.hpp>

#include "common.hpp"

const cv::String keys =
	"{help h usage ? |      | print this message   }"
	"{m              |      | draw a 3d model else draw XYZ axis.}"
	"{c              |      | The input is a camera idx.}"
	"{i              |      | Render an image on the board.}"
	"{v              |      | Render a video on the board.}"
	"{@rows          |<none>| board rows.}"
	"{@cols          |<none>| board cols.}"
	"{@size          |<none>| board squared side size.}"
	"{@intrinsics    |<none>| intrinsics filename.}"
	"{@input         |<none>| input stream (filename or camera idx)}"
	;

int
main (int argc, char* const* argv)
{
	int retCode=EXIT_SUCCESS;

	try
	{

		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Do augmented reality on a input stream.");
		if (parser.has("help"))
		{
			parser.printMessage();
			return EXIT_SUCCESS;
		}
		int rows = parser.get<int>("@rows");
		int cols = parser.get<int>("@cols");
		float size = parser.get<float>("@size");
		std::string intrinsics_file = parser.get<std::string>("@intrinsics");        
		bool is_camera = parser.has("c");
		bool draw_axis = !parser.has("m");
		int camera_idx = -1;
		std::string input_file;
		if (is_camera)
			camera_idx = parser.get<int>("@input");
		else
			input_file = parser.get<std::string>("@input");
		cv::Mat projected_image;
		if (parser.has("i"))
		{
			//TODO: load the image to be projected on the board.
			projected_image = cv::imread(parser.get<cv::String>("i"), cv::IMREAD_UNCHANGED);
		}
		cv::VideoCapture projected_video;
		if (parser.has("v"))
		{
			//TODO: load the video to be projected on the board.
			projected_video.open(parser.get<cv::String>("v"));
		}
		if (!parser.check())
		{
			parser.printErrors();
			return EXIT_FAILURE;
		}
		
		cv::VideoCapture cap;
		if (is_camera)
			cap.open(camera_idx);
		else
			cap.open(input_file);

		if (!cap.isOpened())
		{
			std::cerr << "Error: could not open the input stream!" << std::endl;
			return EXIT_FAILURE;
		}

		cv::Mat M; //camera matrix
		cv::Mat dist_coeffs; //distortion coefficients.
		int image_width, image_height; //input image size.
		float rep_error; //calibration reprojection error.        
		if (!fsiv_load_intrinsic_data(intrinsics_file, rep_error, image_height, image_width, M, dist_coeffs))
		{
			std::cerr << "Error: could not load intrinsics data!" << std::endl;
			return EXIT_FAILURE;
		}
		std::cerr << "Using a calibration with reprj. error= " << rep_error << std::endl;

		cv::Mat input_frame; //Will have the current input frame.
		cv::Mat input_frame_gray; //Will have a gray version of the current input frame.

		//The input color espace depends on the type of input.
		int color_2_gray = cv::COLOR_BGR2GRAY; //For graphics video files.
		if (is_camera)
			color_2_gray = cv::COLOR_RGB2GRAY; //Hardware camera.

		//The 3d world coordinates of the board inner corners.
		std::vector<cv::Point3f> obj_points = fsiv_compute_object_points(rows-1, cols-1, size);

		//Will have the 2d images of the outter four board's corners.
		std::vector<cv::Point2f> board_corners(4);

		//useful when refine coorners coordinates.
		const cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 30, 0.001);

		cv::Mat rvec = cv::Mat::zeros(3, 1, CV_32FC1); //Will have the camera's pose: rotation vector
		cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32FC1); //Will have the camera's pose: traslation vector

		std::vector<cv::Point2f> img_points; //Will have the 2d images of the board 3d corners in current frame.

		cv::namedWindow("VIDEO");
		int wait_time = (is_camera ? 20 : 1000/15.0); //for a video file we assume 15fps rate.
		int key = 0;

		while (key!=27)
		{            
			//For each input frame, find the chessboard (hint use also cv::CALIB_CB_FAST_CHECK).
			//if the board was found, refine the corners and compute the camera pose using cv::solvePnP,
			//then draw something using the intrinsics/extrinsics parameters.

			//TODO: Read the next frame from input stream in the input_frame variable.
			cap >> input_frame;

			if (input_frame.empty())
				break; //If the the read image is empty, the video is finished, so go out.

			//TODO: find the chessboard and rought estimate its inner corners.
			//Complete the following code. Variable was_found must be true if the chessboard was found.
			bool was_found = cv::findChessboardCorners(
								input_frame, 
								cv::Size(cols - 1, rows - 1), 
								board_corners,
								cv::CALIB_CB_FAST_CHECK
							);
			//

			if (was_found)
			{
				//TODO: Get a gray version of the input frame in the input_frame_gray variable. Use the
				//color_2_gray variable to control the kind of conversion to do.
				cv::cvtColor(input_frame, input_frame_gray, color_2_gray);

				//TODO: using the gray version, refine the inner corners to subpixel coordinates.
				//Use the criteria varaible as term_critera parameter.
				cv::cornerSubPix(
					input_frame_gray,
					board_corners,
					cv::Size(11, 11), cv::Size(-1, -1),
					criteria
				);

				//TODO: estimate the camera pose.
				cv::solvePnP(obj_points, board_corners, M, dist_coeffs, rvec, tvec);

				//TODO: if we want to render a video on the board,
				//take the next frame to be rendered using projected_image variable.
				if (projected_video.isOpened())
					projected_video >> projected_image;

				if (!projected_image.empty()) {
					//TODO: save on board_corners vector the four 2d images of outter corners.
					//Follow the sequence up-left, up-right, down-left, down-right.
					fsiv_project_image(projected_image, board_corners, input_frame);
				}
				else if (draw_axis)
					fsiv_draw_axes(M, dist_coeffs, rvec, tvec, size, input_frame);
				else
					fsiv_draw_3d_model(M, dist_coeffs, rvec, tvec, size, input_frame);
			}
			cv::imshow("VIDEO", input_frame);
			key = cv::waitKey(wait_time) & 0xff;
		}
		cv::destroyAllWindows();
	}
	catch (std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
	return retCode;
}
