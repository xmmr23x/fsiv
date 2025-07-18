#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include "common_code.hpp"
#include <iostream>

std::vector<cv::Point3f>
fsiv_generate_3d_calibration_points(const cv::Size& board_size,
									float square_size)
{
	std::vector<cv::Point3f> ret_v;
	//TODO
	//Remenber: the first inner point has (1,1) in board coordinates.
	for (size_t i = 1; i <= board_size.height; i++)
		for (size_t j = 1; j <= board_size.width; j++)
			ret_v.push_back(cv::Point3f(j * square_size, i * square_size, 0));

	//
	CV_Assert(ret_v.size()==board_size.width*board_size.height);
	return ret_v;
}


bool
fsiv_find_chessboard_corners(const cv::Mat& img, const cv::Size &board_size,
							 std::vector<cv::Point2f>& corner_points,
							 const char * wname)
{
	CV_Assert(img.type()==CV_8UC3);
	bool was_found = false;
	//TODO
	was_found = cv::findChessboardCorners(img, board_size, corner_points, cv::CALIB_CB_FAST_CHECK);

	if (was_found) {
		cv::Mat gray;
		cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);

		const cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 30, 0.001);

		cv::cornerSubPix(gray,corner_points,cv::Size(5, 5), cv::Size(-1, -1),criteria);
	}


	//
	return was_found;
}

float
fsiv_calibrate_camera(const std::vector<std::vector<cv::Point2f>>& _2d_points,
					  const std::vector<std::vector<cv::Point3f>>& _3d_points,
					  const cv::Size &camera_size,
					  cv::Mat& camera_matrix,
					  cv::Mat& dist_coeffs,
					  std::vector<cv::Mat>* rvecs,
					  std::vector<cv::Mat>* tvecs)
{
	CV_Assert(_3d_points.size()>=2 && _3d_points.size()==_2d_points.size());
	float error=0.0;
	//TODO
	std::vector <cv::Mat> rvec, tvec;

	error = cv::calibrateCamera(
		_3d_points,
		_2d_points, 
		camera_size, 
		camera_matrix, 
		dist_coeffs,
		rvec,
		tvec
	);

	if (rvecs != nullptr) *rvecs = rvec;
	if (tvecs != nullptr) *tvecs = tvec;

	//
	CV_Assert(camera_matrix.rows==camera_matrix.cols &&
			  camera_matrix.rows == 3 &&
			  camera_matrix.type()==CV_64FC1);
	CV_Assert((dist_coeffs.rows*dist_coeffs.cols) == 5 &&
			  dist_coeffs.type()==CV_64FC1);
	CV_Assert(rvecs==nullptr || rvecs->size()==_2d_points.size());
	CV_Assert(tvecs==nullptr || tvecs->size()==_2d_points.size());
	return error;
}

void fsiv_compute_camera_pose(const std::vector<cv::Point3f> &_3dpoints,
							  const std::vector<cv::Point2f> &_2dpoints,
							  const cv::Mat& camera_matrix,
							  const cv::Mat& dist_coeffs,
							  cv::Mat& rvec,
							  cv::Mat& tvec)
{
	CV_Assert(_3dpoints.size()>=4 && _3dpoints.size()==_2dpoints.size());
	//TODO

	cv::solvePnP(_3dpoints, _2dpoints, camera_matrix, dist_coeffs, rvec, tvec);

	//
	CV_Assert(rvec.rows==3 && rvec.cols==1 && rvec.type()==CV_64FC1);
	CV_Assert(tvec.rows==3 && tvec.cols==1 && tvec.type()==CV_64FC1);
}

void
fsiv_draw_axes(cv::Mat& img,               
			   const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs,
			   const cv::Mat& rvec, const cv::Mat& tvec,
			   const float size, const int line_width)
{
	//TODO
	std::vector<cv::Point2f> img_points;
	std::vector<cv::Point3f> obj_points;

	obj_points.push_back(cv::Point3f(0, 0, 0));
	obj_points.push_back(cv::Point3f(0, size, 0));
	obj_points.push_back(cv::Point3f(size, 0, 0));
	obj_points.push_back(cv::Point3f(0, 0, -size));

	projectPoints(obj_points, rvec, tvec, camera_matrix, dist_coeffs, img_points);

	line(img, img_points[0], img_points[2], cv::Scalar(0, 0, 255), line_width);
	line(img, img_points[0], img_points[1], cv::Scalar(0, 255, 0), line_width);
	line(img, img_points[0], img_points[3], cv::Scalar(255, 0, 0), line_width);


	//
}

void
fsiv_save_calibration_parameters(cv::FileStorage& fs,
								const cv::Size & camera_size,
								float error,
								const cv::Mat& camera_matrix,
								const cv::Mat& dist_coeffs,
								 const cv::Mat& rvec,
								 const cv::Mat& tvec)
{
	CV_Assert(fs.isOpened());
	CV_Assert(camera_matrix.type()==CV_64FC1 && camera_matrix.rows==3 && camera_matrix.cols==3);
	CV_Assert(dist_coeffs.type()==CV_64FC1 && dist_coeffs.rows==1 && dist_coeffs.cols==5);
	CV_Assert(rvec.type()==CV_64FC1 && rvec.rows==3 && rvec.cols==1);
	CV_Assert(tvec.type()==CV_64FC1 && tvec.rows==3 && tvec.cols==1);
	//TODO
	fs << "image-width" << camera_size.width;
	fs << "image-height" << camera_size.height;
	fs << "error" << error;
	fs << "camera-matrix" << camera_matrix;
	fs << "distortion-coefficients" << dist_coeffs;
	fs << "rvec" << rvec;
	fs << "tvec" << tvec;

	//
	CV_Assert(fs.isOpened());
	return;
}

void
fsiv_load_calibration_parameters(cv::FileStorage &fs,
								 cv::Size &camera_size,
								 float& error,
								 cv::Mat& camera_matrix,
								 cv::Mat& dist_coeffs,
								 cv::Mat& rvec,
								 cv::Mat& tvec)
{
	CV_Assert(fs.isOpened());
	//TODO
	int h, w;
	fs["image-height"] >> h;
	fs["image-width"] >> w;
	camera_size = cv::Size(w, h);

	fs["error"] >> error;
	fs["camera-matrix"] >> camera_matrix;
	fs["distortion-coefficients"] >> dist_coeffs;
	fs["rvec"] >> rvec;
	fs["tvec"] >> tvec;

	//
	CV_Assert(fs.isOpened());
	CV_Assert(camera_matrix.type()==CV_64FC1 && camera_matrix.rows==3 && camera_matrix.cols==3);
	CV_Assert(dist_coeffs.type()==CV_64FC1 && dist_coeffs.rows==1 && dist_coeffs.cols==5);
	CV_Assert(rvec.type()==CV_64FC1 && rvec.rows==3 && rvec.cols==1);
	CV_Assert(tvec.type()==CV_64FC1 && tvec.rows==3 && tvec.cols==1);
	return;
}

void
fsiv_undistort_image(const cv::Mat& input, cv::Mat& output,
					 const cv::Mat& camera_matrix,
					 const cv::Mat& dist_coeffs)
{
	//TODO
	//Hint: use cv::undistort.
	output = input.clone();
	cv::undistort(input, output, camera_matrix, dist_coeffs);
	//
}

void
fsiv_undistort_video_stream(cv::VideoCapture&input_stream,
							cv::VideoWriter& output_stream,
							const cv::Mat& camera_matrix,
							const cv::Mat& dist_coeffs,
							const int interp,
							const char * input_wname,
							const char * output_wname,
							double fps)
{
	CV_Assert(input_stream.isOpened());
	CV_Assert(output_stream.isOpened());
	//TODO
	//Hint: to speed up, first compute the transformation maps
	//(one time only at the beginning using cv::initUndistortRectifyMap)
	// and then only remap (cv::remap) the input frame with the computed maps.

	//
	CV_Assert(input_stream.isOpened());
	CV_Assert(output_stream.isOpened());
}
