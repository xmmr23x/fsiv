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
	"{verbose        |      | activate verbose mode.}"
	"{i intrinsics   |      | Calibrate only extrinsics parameters. Using intrinsics from given file (-i=intr-file).}"
	"{s size         |<none>| square size.}"
	"{r rows         |<none>| number of board's rows.}"
	"{c cols         |<none>| number of board's cols.}"
	"{@output        |<none>| filename for output intrinsics file.}"
	"{@input1        |<none>| first board's view.}"
	"{@input2        |      | second board's view.}"
	"{@inputn        |      | ... n-idx board's view.}"
	;

int
main (int argc, char* const* argv)
{
	int retCode=EXIT_SUCCESS;

	try {        
		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Calibrate the intrinsics parameters of a camera.");
		if (parser.has("help"))
		{
			parser.printMessage();
			return EXIT_SUCCESS;
		}
		float square_size = parser.get<float>("s");
		int rows = parser.get<int>("r");
		int cols = parser.get<int>("c");
		bool verbose = parser.has("verbose");
		std::string output_fname = parser.get<cv::String>("@output");
		if (!parser.check())
		{
			parser.printErrors();
			return EXIT_FAILURE;
		}

		//Get the inputs.
		//find the second argument without '-' at begin.
		int input = 1;
		bool found = false;
		while (input<argc && !found)
			found = argv[input++][0] != '-';
		CV_Assert(input<argc);
		std::vector<std::string> input_fnames;        
		for (; input<argc; ++input)
			input_fnames.push_back(std::string(argv[input]));

		//TODO
		cv::namedWindow("INPUT");
		cv::Mat camera_matrix, dist_coeffs;
		std::vector<std::vector<cv::Point2f>> _2d_points(input_fnames.size());
		std::vector<std::vector<cv::Point3f>> _3d_points(input_fnames.size());
		std::vector<cv::Point3f> _3d_point;
		std::vector <cv::Mat> rvecs, tvecs, in, axes;

		for (int i = 0; i < input_fnames.size(); ++i)
				in.push_back(cv::imread(input_fnames[i], cv::IMREAD_UNCHANGED));

		_3d_point  = fsiv_generate_3d_calibration_points(cv::Size(cols-1, rows-1), square_size);
		axes = in;

		if (parser.has("i"))
		{
			//TODO
			//Make extrinsic calibration.
			//Remenber: only one view is needed.
			std::string intrinsics_file = parser.get<std::string>("i");
			cv::FileStorage fs(intrinsics_file, cv::FileStorage::READ);
			float error;
			cv::Size camera_size;
			cv::Mat rvec, tvec;


			fsiv_load_calibration_parameters(
				fs, camera_size, error, camera_matrix,
				dist_coeffs, rvec, tvec
			);

			bool was_found = fsiv_find_chessboard_corners(
				in[0], cv::Size(cols-1, rows-1),
				_2d_points[0], "IN"
			);

			if (was_found) fsiv_compute_camera_pose(
				_3d_point,
				_2d_points[0],
				camera_matrix,
				dist_coeffs,
				rvec,
				tvec
			);

			rvecs.push_back(rvec);
			tvecs.push_back(tvec);


			//
			if (verbose)
			{
				//TODO
				//Show WCS axis.
				fsiv_draw_axes(
					axes[0], 
					camera_matrix, 
					dist_coeffs,
					rvecs[0],
					tvecs[0],
					square_size,
					3
				);

				cv::imshow ("INPUT", axes[0]);
				cv::waitKey()&0xff;



				//
			}
		}
		else
		{
			//TODO
			//Make an intrisic calibration.
			//Remember: For each view (at least two) you must find the
			//chessboard to get the 3D -> 2D matches.
			cv::FileStorage fs(output_fname, cv::FileStorage::WRITE);

			for (int i = 0; i < input_fnames.size(); ++i) {
				fsiv_find_chessboard_corners(
					in[i], cv::Size(cols-1, rows-1),
					_2d_points[i], "IN"
				);

				_3d_points[i] = _3d_point;
			}

			float error = fsiv_calibrate_camera(
				_2d_points,
				_3d_points, 
				cv::Size(rows-1, cols-1),
				camera_matrix,
				dist_coeffs,
				&rvecs,
				&tvecs
			);
			//
			//
			fsiv_save_calibration_parameters(
				fs,
				cv::Size(rows-1, cols-1),
				error,
				camera_matrix,
				dist_coeffs,
				rvecs[0],
				tvecs[0]
			);

			if (verbose)
			{
				//TODO
				//Show WCS axis on each pattern view.
				for (int i = 0; i < input_fnames.size(); ++i) {
					fsiv_draw_axes(
						axes[i], 
						camera_matrix, 
						dist_coeffs,
						rvecs[i],
						tvecs[i],
						square_size,
						2
					);

					cv::imshow ("INPUT", axes[i]);
					cv::waitKey()&0xff;
				}
				//
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
	return retCode;
}
