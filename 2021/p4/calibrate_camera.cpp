/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>
#include <sstream>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "common.hpp"

const cv::String keys =
    "{help h usage ? |      | print this message   }"
    "{@rows          |<none>| board rows.}"
    "{@cols          |<none>| board cols.}"
    "{@size          |<none>| board squared side size.}"
    "{@intrinsics    |<none>| intrinsics filename.}"
    "{@inputs        |<none>| board views}"
    ;

int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try
    {

        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Calibrate a camera.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }
        int rows = parser.get<int>("@rows");
        int cols = parser.get<int>("@cols");
        float size = parser.get<float>("@size");
        std::string intrinsics_file = parser.get<std::string>("@intrinsics");
        if (argc<6)
        {
            std::cerr << "Error: you must give several board views."
                      << std::endl;
            parser.printMessage();
            return EXIT_FAILURE;
        }
        std::vector<cv::Mat> input_imgs;
        for (int i=5; i<argc; ++i)
        {
            //TODO: load, pushing back, the input images. Use argv[i] for the filename.
            //Force color mode when reading the images.
            //Control that each image is loaded.
        }
        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }

        std::vector<cv::Point3f> world_coordinates =
                fsiv_compute_object_points(rows, cols, size);

        std::vector<std::vector<cv::Point3f>> obj_points; //The Worl 3d coordinates per view.
        std::vector<std::vector<cv::Point2f>> img_points; //The projected 2d coordinates per view.

        const cv::TermCriteria term_criteria(cv::TermCriteria::EPS +
                                             cv::TermCriteria::MAX_ITER,
                                             30, 0.001); //Useful to refine the 2d coordinates.

        for (size_t v=0; v<input_imgs.size(); ++v)
        {
            std::cout << "Using view number: " << v << std::endl;
            std::vector<cv::Point2f> corners;

            //TODO: find the chessboard and rought estimate its inner corners.
            //Complete the following code. Variable was_found must be true if the chessboard was found.
            bool was_found = false;
            //

            if (was_found)
            {
                cv::Mat gray_img;
                //TODO: Get a gray version of the input frame in the gray_img variable.

                //TODO: using the gray version, refine the inner corners to subpixel coordinates.
                //Use the criteria varaible as term_critera parameter.

                //TODO: draw the chessboar corners detected indicating,
                //that the chessboard was found.
            }
            else
                //TODO: draw the chessboar corners detected indicating,
                //that the chessboard not was found.
                ;
                //

            std::ostringstream win_name;
            win_name.str("");
            win_name << "VIEW " << v;
            cv::imshow(win_name.str(), input_imgs[v]);
            int key = cv::waitKey(0) & 0xFF;
            cv::destroyWindow(win_name.str());
            if (was_found && key != 27)
            {
                //TODO: add the objet points and the images points detected
                //to their correspoding vectors.
            }
        }

        //Now calibrate the camera.
        if (img_points.size()>0)
        {
            std::cout << "Calibrating the camera with "
                      << img_points.size() << " valid views." << std::endl;

            //Calibration parameters.
            cv::Mat M; //Camera matrix.
            cv::Mat dist_coeffs; //distortion coefficents.
            std::vector<cv::Mat> rvecs; //rotation vector for each view.
            std::vector<cv::Mat> tvecs; //traslation vector for each view.
            float error = 0.0; //Calibration reprojection error achived.

            //TODO: calibrate the camera. using the above variables to get the results.


            std::cerr << "Calibration reprojection error: "
                      << error << std::endl;

            if (! fsiv_write_intrinsic_data(intrinsics_file, error,
                                            input_imgs[0].rows,
                                            input_imgs[0].cols, M, dist_coeffs))
            {
                std::cerr << "Error: could not write the intrinsics params "
                             "to file '" << intrinsics_file << "'."
                          << std::endl;
                return EXIT_FAILURE;
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
