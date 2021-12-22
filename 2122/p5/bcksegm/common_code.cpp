// ----------------------------------------
// seglib.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include "common_code.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>

void fsiv_remove_segmentation_noise(cv::Mat & img, int r) {
	CV_Assert(img.type()==CV_8UC1);
	CV_Assert(r>0);
	
	//TODO
	//Apply a closing+opening using a square structuring element with radius r.
	cv::Size s(2*r+1,2*r+1);
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, s);
	cv::morphologyEx(img, img, cv::MORPH_CLOSE, k);
	cv::morphologyEx(img, img, cv::MORPH_OPEN, k);
	//
}

void fsiv_segm_by_dif(
	const cv::Mat & prevFrame, 
	const cv::Mat & curFrame,
	cv::Mat & difimg, 
	int thr, 
	int r
) {
	CV_Assert(prevFrame.type()==CV_8UC3 && prevFrame.type()==curFrame.type());
	CV_Assert(prevFrame.size()==prevFrame.size());

	//TODO
	//Remenber: use >= to compare with 'thr'.
	cv::Mat prev_gray, cur_gray;
	cv::cvtColor(prevFrame, prev_gray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(curFrame, cur_gray, cv::COLOR_BGR2GRAY);

	difimg = cv::abs(prev_gray - cur_gray) >= thr;

	if (r > 0)
		fsiv_remove_segmentation_noise(difimg, r);
	//

	CV_Assert(difimg.type()==CV_8UC1);
	CV_Assert(difimg.size()==curFrame.size());
}

void fsiv_apply_mask(
	const cv::Mat & frame, 
	const cv::Mat & mask,
	cv::Mat & outframe
) {
	CV_Assert(frame.type()==CV_8UC1 || frame.type()==CV_8UC3);
	CV_Assert(mask.type()==CV_8UC1);
	CV_Assert(frame.size()==mask.size());

	//TODO
	//Remenber: frame could have 3 channels.
	frame.copyTo(outframe, mask);
	//

	CV_Assert(outframe.type()==frame.type());
	CV_Assert(outframe.size()==frame.size());
}

bool
fsiv_learn_gaussian_model(
	cv::VideoCapture & input,
	cv::Mat & mean,
	cv::Mat & variance,
	int num_frames,
	int gauss_r,
	const char * wname
) {
	CV_Assert(input.isOpened());
	bool was_ok = true;

	//TODO
	// Remenber you can compute the variance as:
	// varI = sum_n{I^2}/n - meanI²
	// Hint: convert to input frames to float [0,1].
	// Hint: use cv::accumulate() and cv::accumulateSquare().
	cv::Mat aux, mean2;
	int i = num_frames;
	cv::Size s(2*gauss_r+1,2*gauss_r+1);

	// inicializacion de las imagenes necesarias para calcular la media
	was_ok   = input.read(aux);
	mean     = cv::Mat::zeros(aux.size(), CV_32FC3);
	variance = cv::Mat::zeros(aux.size(), CV_32FC3);

	while (was_ok && i-- > 0) {
		std::cerr << i << std::endl;
		if (gauss_r > 0)
			cv::GaussianBlur(aux, aux, s, 0);

		aux.convertTo(aux, CV_32F, 1.0/255.0);
		cv::accumulate(aux, mean);
		cv::accumulateSquare(aux, variance);
		input.read(aux);
	}

	mean = mean/num_frames;
	variance = (variance/num_frames) - mean.mul(mean);
	//
	CV_Assert(!was_ok || mean.type()==CV_32FC3);
	CV_Assert(!was_ok || variance.type()==CV_32FC3);
	return was_ok;
}

void
fsiv_segm_by_gaussian_model(
	const cv::Mat & frame,
	cv::Mat & mask,
	const cv::Mat & mean,
	const cv::Mat & variance, 
	float k, 
	int r
) {
	CV_Assert(frame.type()==CV_32FC3);

	//TODO
	//Remenber: a point belongs to the foreground (255) if |mean-I| >= k*stdev
	cv::Mat stdev, mask_[3];

	cv::sqrt(variance, stdev);

	mask = cv::abs(mean - frame) >= k*stdev;

	cv::split(mask, mask_);
	cv::bitwise_or(mask_[0], mask_[1], mask);
	cv::bitwise_or(mask, mask_[2], mask);

	if (r > 0)
		fsiv_remove_segmentation_noise(mask, r);

	//

	CV_Assert(mask.type()==CV_8UC1);
}

void
fsiv_update_gaussian_model(
	const cv::Mat & frame,
	const cv::Mat & mask,
	unsigned long frame_count,
	cv::Mat & mean,
	cv::Mat & variance,
	float alpha,
	unsigned short_term_update_period,
	unsigned long_term_update_period
) {
	CV_Assert(frame.type()==CV_32FC3);
	CV_Assert(mask.type()==CV_8UC1);
	CV_Assert(mean.type()==CV_32FC3);
	CV_Assert(variance.type()==CV_32FC3);
	CV_Assert(frame.size()==mask.size());

	//TODO
	//Remember: In the short term updating you must update the model using
	//the background only (not mask).
	//However in the long term updating you must update
	//the model using both background and foreground (without mask).
	//Hint: a period is met when (idx % period) == 0
	//Hint: use accumulateWeighted to update the model.
	cv::Mat not_mask, frame2;
	cv::pow(frame,2,frame2);

	if (frame_count % short_term_update_period == 0) {
		cv::bitwise_not(mask, not_mask);
		cv::accumulateWeighted(frame, mean, alpha, not_mask);
		cv::accumulateWeighted(frame2, variance, alpha, not_mask);
	}

	else if (frame_count % long_term_update_period == 0) {
		cv::accumulateWeighted(frame, mean, alpha);
		cv::accumulateWeighted(frame2, variance, alpha);
	}


	//
}
