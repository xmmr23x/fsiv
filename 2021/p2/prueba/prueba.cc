#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
int main( int argc, char** argv )
{
	Mat imgA, grayImg;
	if( argc != 2 || !(imgA=imread(argv[1], 1)).data )
		return -1;
	imshow("img1", imgA);

	//variables preparing
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int hbins = 255; //histogram x axis size, that is histSize, 
					//ex) 2 -> 0~128, 129~256, ex)16 -> 0 ~ 15, 16 ~ 31..., 
	int channels[] = { 0 }; //index of channel
	int histSize[] = { hbins };
	float hranges[] = { 0, 255 };
	const float* ranges[] = { hranges };

	MatND HistB, HistG, HistR;
	//split rgb
	std::vector<Mat> bgr_planes;
	split(imgA, bgr_planes);

	//cal histogram & normalization
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	calcHist(&bgr_planes[0], 1, 0, Mat(), HistB, 1, histSize, ranges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), HistG, 1, histSize, ranges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), HistR, 1, histSize, ranges, true, false);

	normalize(HistB, HistB, 0, 255, CV_MINMAX);
	normalize(HistG, HistG, 0, 255, CV_MINMAX);
	normalize(HistR, HistR, 0, 255, CV_MINMAX);

	// Draw the histograms for B, G and R
	int hist_w = 500; int hist_h = 255;
	int ratio = cvRound((double)hist_w / hbins);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	int x1, y1;
	int x2, y2;
	for (int i = 1; i < hbins; i++)
	{
		x1 = ratio*(i - 1);
		y1 = hist_h - cvRound(HistB.at<float>(i - 1));
		x2 = ratio*(i);
		y2 = hist_h - cvRound(HistB.at<float>(i));

		//Blue
		line(histImage, Point(x1,y1), Point(x2,y2),
			CV_RGB(0, 0, 255), 2, 8, 0);

		//Green
		y1 = hist_h - cvRound(HistG.at<float>(i - 1));      
		y2 = hist_h - cvRound(HistG.at<float>(i));
		line(histImage, Point(x1, y1), Point(x2, y2),
			CV_RGB(0, 255, 0), 2, 8, 0);

		//Red
		y1 = hist_h - cvRound(HistR.at<float>(i - 1));
		y2 = hist_h - cvRound(HistR.at<float>(i));
		line(histImage, Point(x1, y1), Point(x2, y2),
			CV_RGB(255, 0, 0), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
	waitKey(0);

	return 0;
}