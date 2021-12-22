#include "common.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
/*
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
*/

bool fsiv_load_intrinsic_data(
	const std::string& filename,
	float & error,
	int & height,
	int & width,
	cv::Mat & M,
	cv::Mat & dist_coeffs
) {
	//TODO
	//Hint: User cv::FileStorage class to manage this.
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened()) return false;

	fs["error"] >> error;
	fs["image-heigh"] >> height;
	fs["image-width"] >> width;
	fs["camera-matrix"] >> M;
	fs["distortion-coefficients"] >> dist_coeffs;

	return true;
}

bool fsiv_write_intrinsic_data(
	const std::string& filename,
	const float error,
	const int height,
	const int width,
	const cv::Mat M,
	const cv::Mat dist_coeffs
) {
	bool ret_val = true;

	//TODO
	//Hint: User cv::FileStorage class to manage this.

	return ret_val;
}


std::vector<cv::Point3f> fsiv_compute_object_points(
	const int rows,
	const int cols,
	const float size
) {
	std::vector<cv::Point3f> obj_points;

	//TODO
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			obj_points.push_back(cv::Point3f(j * size, i * size, 0));

	return obj_points;
}

void fsiv_draw_axes(
	const cv::Mat& M, 
	const cv::Mat& dist_coeffs,
	const cv::Mat& rvec, 
	const cv::Mat& tvec,
	const float size,
	cv::Mat img
) {
	CV_Assert(img.type()==CV_8UC3);

	//TODO
	std::vector<cv::Point2f> img_points;
	std::vector<cv::Point3f> obj_points;

	obj_points.push_back(cv::Point3f(-size, -size, 0));
	obj_points.push_back(cv::Point3f(0, -size, 0));
	obj_points.push_back(cv::Point3f(-size, 0, 0));
	obj_points.push_back(cv::Point3f(-size, -size, -size));

	projectPoints(obj_points, rvec, tvec, M, dist_coeffs, img_points);

	line(img, img_points[0], img_points[1], cv::Scalar(255, 0, 0), 3);
	line(img, img_points[0], img_points[2], cv::Scalar(0, 255, 0), 3);
	line(img, img_points[0], img_points[3], cv::Scalar(0, 0, 255), 3);
}


void fsiv_draw_3d_model(
	const cv::Mat& M, 
	const cv::Mat& dist_coeffs,
	const cv::Mat& rvec, const cv::Mat& tvec,
	const float size,
	cv::Mat img
) {
	CV_Assert(img.type()==CV_8UC3);

	//TODO
	std::vector<cv::Point2f> img_points;
	std::vector<cv::Point3f> obj_points;

	// base
	obj_points.push_back(cv::Point3f(2 * size, 2 * size, 0));
	obj_points.push_back(cv::Point3f(2 * size, 3 * size, 0));
	obj_points.push_back(cv::Point3f(3 * size, 2 * size, 0));
	obj_points.push_back(cv::Point3f(3 * size, 3 * size, 0));

	// punto superior
	obj_points.push_back(cv::Point3f(2.5 * size, 2.5 * size, -size));

	projectPoints(obj_points, rvec, tvec, M, dist_coeffs, img_points);

	// base
	line(img, img_points[0], img_points[1], cv::Scalar(120, 255, 90), 3);
	line(img, img_points[0], img_points[2], cv::Scalar(120, 255, 90), 3);
	line(img, img_points[1], img_points[3], cv::Scalar(120, 255, 90), 3);
	line(img, img_points[2], img_points[3], cv::Scalar(120, 255, 90), 3);

	// punto superior
	line(img, img_points[0], img_points[4], cv::Scalar(120, 255, 90), 3);
	line(img, img_points[1], img_points[4], cv::Scalar(120, 255, 90), 3);
	line(img, img_points[2], img_points[4], cv::Scalar(120, 255, 90), 3);
	line(img, img_points[3], img_points[4], cv::Scalar(120, 255, 90), 3);
}

void fsiv_project_image(
	const cv::Mat& input,
	const std::vector<cv::Point2f>& image_points,
	cv::Mat& output
) {
	std::vector<cv::Point2f> input_corners(4);
	std::vector<cv::Point2f> image_points_i(4);

	//TODO
	//Hint: output is a combination of the input image masked with
	//the warpped image.
	input_corners[0] = cv::Point2f(0.0, 0.0);
	input_corners[1] = cv::Point2f(input.cols-1, 0.0);
	input_corners[2] = cv::Point2f(0.0, input.rows-1);
	input_corners[3] = cv::Point2f(input.cols-1, input.rows-1);

	image_points_i[0] = image_points[0];
	image_points_i[1] = image_points[4];
	image_points_i[2] = image_points[15];
	image_points_i[3] = image_points[19];

	cv::Mat rotate, perspective_image;
	rotate = cv::getPerspectiveTransform(input_corners, image_points_i);

	cv::Mat mask(input.size(), input.type(), cv::Scalar::all(255));
	cv::warpPerspective(mask, mask, rotate, output.size());

	cv::warpPerspective(input, perspective_image, rotate, output.size());
	perspective_image.copyTo(output, mask);
}
