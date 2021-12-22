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
	"{r              |1     | Window's radius. Default 1.}"
	"{g              |1.0   | Enhance's gain. Default 1.0}"
	"{c              |      | Use circular convolution.}"
	"{f              |0     | Filter type: 0->Box, 1->Gaussian. Default 0.}"
	"{@input         |<none>| input image.}"
	"{@output        |<none>| output image.}"
	;


/**
 * @brief Return a box filter.
 * @arg[in] r is the filter's radius.
 * @pre r>0;
 * @post retV.rows==retV.cols==2*r+1
 * @post cv::sum(retV)==1.0
 */
cv::Mat fsiv_create_box_filter(const int r) {
	CV_Assert(r>0);

	//TODO
	int rad = 2*r+1;
	cv::Mat box_filter(rad, rad, CV_32FC1, cv::Scalar(1.0/(rad*rad)));

	return box_filter;
}

/**
 * @brief Return a Gaussian filter.
 * @arg[in] r is the filter's radius.
 * @pre r>0;
 * @post retV.rows==retV.cols==2*r+1
 * @post cv::sum(retV)==1.0
 */
cv::Mat fsiv_create_gaussian_filter(const int r) {
	CV_Assert(r>0);

	//TODO
	int d   = 2*r+1;
	float s = pow(d/6.0, 2);
	float m = 0;
	float c = 1.0/(2*3.14*s);

	cv::Mat g_filter(d, d, CV_32FC1, cv::Scalar(1.0));

	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < d; ++j) {
			int n                    = -(pow(i-r/2, 2) + pow(j-r/2, 2));
			g_filter.at<float>(i, j) = c * pow(2.7182, n/(2*s));
			m                        += g_filter.at<float>(i, j);
		}
	}

	return g_filter/m;
}

/**
 * @brief Expand an image with zero padding.
 * @arg[in] in is the input image.
 * @arg[in] r is the window's radius to expand.
 * @return the expanded image.
 * @pre !in.empty()
 * @pre r>0
 * @post retV.type()==in.type()
 * @post retV.rows==in.rows+2*r
 * @post retV.cols==in.cols+2*r
 */
cv::Mat fsiv_fill_expansion(cv::Mat const& in, const int r)
{
	CV_Assert(!in.empty());
	CV_Assert(r>0);

	//TODO
	cv::Mat img(in.rows + 2*r, in.cols + 2*r, in.type(), cv::Scalar(0));
	in.copyTo(img(cv::Rect(r, r, in.cols, in.rows)));

	return img;
}

/**
 * @brief Circular expansion of an image.
 * @arg[in] in is the input image.
 * @arg[in] r is the window's radius to expand.
 * @return the expanded image.
 * @pre !in.empty()
 * @pre in.type()==CV_32FC1
 * @pre r>0
 * @post retV.type()==in.type()
 * @post retV.rows==in.rows+2*r
 * @post retV.cols==in.cols+2*r
 */
cv::Mat fsiv_circular_expansion(cv::Mat const& in, const int r)
{
	CV_Assert(!in.empty());
	CV_Assert(in.type()==CV_32FC1);
	CV_Assert(r>0);

	//TODO

	return cv::Mat();
}
/**
 * @brief Compute the digital convolution.
 * By default the input image is expanded with zero padding.
 * Note: Code from scracth. Use cv::filter2D() is not allowed.
 * @arg[in] in is the input image.
 * @arg[in] filter is the filter to be applied.
 * @arg[in] circular if it is true, the input image is expanded following a circular fashion.
 * @pre !in.empty() && !filter.empty()
 * @pre (filter.rows % 2) >0
 * @pre (filter.cols % 2) >0
 * @pre in.type()==CV_32FC1 && filter.type()==CV_32FC1.
 * @post ret.type()==CV_32FC1 && ret.size == in.size.
 */
cv::Mat fsiv_convolve(cv::Mat const& in, cv::Mat const& filter, const bool circular=false)
{
	CV_Assert(!in.empty() && !filter.empty());
	CV_Assert((filter.rows%2 >0 ) && (filter.cols%2 >0));
	CV_Assert(in.type()==CV_32FC1 && filter.type()==CV_32FC1);

	//TODO
	cv::Mat extend = fsiv_fill_expansion(in, (filter.cols-1)/2);
	cv::Mat out    = cv::Mat::zeros(in.size(), CV_32FC1);

	for (int i = 0; i < in.rows; ++i) {
		for (int j = 0; j < in.cols; ++j)
			out.at<float>(i,j) = sum(filter.mul(extend(cv::Rect(j, i, filter.cols, filter.rows)))).val[0];
	}

	return out;
}

/**
 * @brief Apply an unsharp mask enhance to the input image.
 * @arg[in] in is the input image.
 * @arg[in] g is the enhance's gain.
 * @arg[in] r is the window's radius.
 * @arg[in] filter_type specifies which filter to use. 0->Box, 1->Gaussian.
 * @arg[in] circular specifies if it is true, it be used circular expansion to do the convolution, else it is used zero padding.
 * @pre !in.empty()
 * @pre in.type()==CV_32FC1
 * @pre g>=0.0
 * @pre r>0
 * @pre filter_type is {0, 1}
 */
cv::Mat fsiv_usm_enhance(cv::Mat  const& in, float g=1.0, int r=1, int filter_type=0, bool circular=false) {
	CV_Assert(!in.empty());
	CV_Assert(in.type()==CV_32FC1);
	CV_Assert(r>0);
	CV_Assert(g>=0.0);

	//TODO
	cv::Mat f;
	if (filter_type == 0) f = fsiv_create_box_filter(r);
	if (filter_type == 1) f = fsiv_create_gaussian_filter(r);

	cv::Mat conv = fsiv_convolve(in, f);
	cv::Mat usm  = (g+1) * in - (g * conv);

	cv::namedWindow("conv");
	imshow("conv", conv);

	return usm;
}

/**
 * @brief Application State.
 * Use this structure to maintain the state of the application
 * that will be passed to the callbacks.
 */
struct UserData
{
	//Use this type if you implement a GUI.
};

/** @brief Standard trackbar callback
 * Use this function an argument for cv::createTrackbar to control
 * the trackbar changes.
 *
 * @arg v give the trackbar position.
 * @arg user_data allow to pass user data to the callback.
 */
void on_change_r(int v, void * user_data_)
{
	UserData * user_data = static_cast<UserData*>(user_data_);
	//Use this callback if you implement a GUI.
}

/**
 * @brief Standard trackbar callback
 * Use this function an argument for cv::createTrackbar to control
 * the trackbar changes.
 *
 * @arg v give the trackbar position.
 * @arg user_data allow to pass user data to the callback.
 */
void on_change_g(int v, void * user_data_)
{
	UserData * user_data = static_cast<UserData*>(user_data_);
	//Use this callback if you implement a GUI.
}

/**
 * @brief Standard trackbar callback
 * Use this function an argument for cv::createTrackbar to control
 * the trackbar changes.
 *
 * @arg v give the trackbar position.
 * @arg user_data allow to pass user data to the callback.
 */
void on_change_f(int v, void * user_data_)
{
	UserData * user_data = static_cast<UserData*>(user_data_);
	//Use this callback if you implement a GUI.
}

/**
 * @brief Standard button callback
 * Use this function an argument for cv::createTrackbar to control
 * the trackbar changes.
 *
 * @arg state give the button's state.
 * @arg user_data allow to pass user data to the callback.
 */
void on_change_c(int state, void *user_data_)
{
	UserData * user_data = static_cast<UserData*>(user_data_);
	//Use this callback if you implement a GUI.
}

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

		//TODO
		//manage the CLI parameters.
		int r   = parser.get<int>("r");
		float g = parser.get<float>("g");
		int f   = parser.get<int>("f");

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

		//TODO
		//Hint: convert the input image to CV_32F.
		cv::Mat out, img, c[3];
		
		if (in.channels() > 1) {
			cvtColor(in, img, CV_BGR2HSV);
			cv::split(img, c);
			c[2].copyTo(img);
		}
		else in.copyTo(img);

		img.convertTo(img, CV_32FC1, 1.0/255.0);
		out = fsiv_usm_enhance(img, g, r, f);
		out.convertTo(out, CV_8UC1, 255.0);

		if (in.channels() > 1) {
			out.copyTo(c[2]);
			cv::merge(c, 3, out);
			cv::cvtColor(out, out, CV_HSV2BGR);
		}

		cv::namedWindow("original");
		cv::imshow("original", in);
		cv::namedWindow("usm");
		cv::imshow("usm", out);
		
		if (cv::waitKey(0) != 27) cv::imwrite(output_n, out);
	}
	catch (std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
	return retCode;
}
