#include "common_code.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cv::Mat fsiv_color_rescaling(const cv::Mat& in, const cv::Scalar& from, const cv::Scalar& to)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;
    //TODO
    //Cuidado con dividir por cero.
    //Evita los bucles.
    cv::Scalar wp(1,1,1);

    if (from[0] != 0 && from[1] != 0 && from[2] != 0) 
        wp = cv::Scalar(to[0]/from[0],to[1]/from[1],to[2]/from[2]);

    out = in.mul(wp);
    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

cv::Mat fsiv_wp_color_balance(cv::Mat const& in)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;
    //TODO
    //Sugerencia: utiliza el espacio de color GRAY para
    //saber la ilumimancia de un pixel.

    // conversion de color a escala de grises
    cv::Mat gray;
    cv::cvtColor(in, gray, cv::COLOR_BGR2GRAY);

    // obtencion del punto mas luminoso
    cv::Point p_max;
    cv::minMaxLoc(in, NULL, NULL, NULL, &p_max);
    cv::Vec3b max_val = in.at<cv::Vec3b>(p_max);

    // valores para calcular wp 
    cv::Scalar white(255,255,255);
    cv::Scalar max(max_val[0],max_val[1],max_val[2]);

    // calculo
    fsiv_color_rescaling();

    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

cv::Mat fsiv_gw_color_balance(cv::Mat const& in)
{
    CV_Assert(in.type()==CV_8UC3);
    cv::Mat out;
    //TODO



    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}

cv::Mat fsiv_color_balance(cv::Mat const& in, float p)
{
    CV_Assert(in.type()==CV_8UC3);
    CV_Assert(0.0f<p && p<100.0f);
    cv::Mat out;
    //TODO
    //Sugerencia: utiliza el espacio de color GRAY para
    //saber la ilumimancia de un pixel.


    //
    CV_Assert(out.type()==in.type());
    CV_Assert(out.rows==in.rows && out.cols==in.cols);
    return out;
}
