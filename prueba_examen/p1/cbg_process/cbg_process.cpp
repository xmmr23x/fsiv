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
    "{i interactive  |      | Activate interactive mode.}"
    "{l luma         |      | process only \"luma\" if color image.}"
    "{c contrast     |1.0   | contrast parameter.}"
    "{b bright       |0.0   | bright parameter.}"
    "{g gamma        |1.0   | gamma parameter.}"
    "{@input         |<none>| input image.}"
    "{@output        |<none>| output image.}"
    ;



int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Ajust the contrast/bright/gamma parameters of an image. (ver 0.0.0)");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }

      cv::String input_name = parser.get<cv::String>(0);
      cv::String output_name = parser.get<cv::String>(1);

      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

      cv::Mat input;
      cv::Mat output;
      cv::namedWindow("ORIGINAL");
      cv::namedWindow("PROCESADA");


      //TODO


      int key = cv::waitKey(0) & 0xff;

      if (key != 27)
      {
          if (!cv::imwrite(output_name, output))
          {
              std::cerr << "Error: could not save the result in file '"<<output_name<<"'."<< std::endl;
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


