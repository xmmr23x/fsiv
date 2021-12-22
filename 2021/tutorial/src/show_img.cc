/*!
  Ejemplo de carga de imagen y su visualización.

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

//Interfaz de línea de comandos.
const cv::String keys =
    "{help h usage ? |      |print this message.}" //opción activada al poner -h, --help, --usage, -?
    "{@image         |<none>|image to show.}" //argumento sin valor por defecto.
    ;

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    

      cv::CommandLineParser parser(argc, argv, keys); //Creo el gestor de la línea de comandos.
      parser.about("Muestra una imagen v1.0.0"); //Se indica el nombre del programa.
      if (parser.has("help")) //El usario ha puesto -h o --help,
      {
          parser.printMessage();
          return 0;
      }
      cv::String img_name = parser.get<cv::String>("@image"); //Obtenermos el argumento @image.

      if (!parser.check()) //Comprueba si hubo algún error en la línea de comandos.
      {
          parser.printErrors();
          return 0;
      }



      //Carga la imagen desde archivo.
      //En funcion de como se compilo opencv podra
      //cargar mas o menos formatos graficos.
      //Lee la documentacion de imread para ver mas detalles.
      cv::Mat img = cv::imread(img_name, cv::IMREAD_UNCHANGED);

      if (img.empty())
      {
         std::cerr << "Error: no he podido abrir el fichero '" << img_name << "'." << std::endl;
         return EXIT_FAILURE;
      }

      //Vamos a mostrar alguna información sobre la imagen cargada.
      std::cout << "Ancho : " << img.cols << std::endl;
      std::cout << "Alto  : " << img.rows << std::endl;
      std::cout << "Número de canales: " << img.channels() << std::endl;
      std::cout << "Profundidad de bit: ";
      switch (img.depth())
      {
      case CV_8S:
          std::cout << " entero 8 bits con signo." << std::endl;
          break;
      case CV_8U:
          std::cout << " entero 8 bits sin signo." << std::endl;
          break;
      case CV_16S:
          std::cout << " entero 16 bits con signo." << std::endl;
          break;
      case CV_16U:
          std::cout << " entero 16 bits sin signo." << std::endl;
          break;
      case CV_32S:
          std::cout << " entero 32 bits con signo." << std::endl;
          break;
      case CV_32F:
          std::cout << " flotante 32 bits." << std::endl;
          break;
      case CV_64F:
          std::cout << " flotante 64 bits." << std::endl;
          break;
      default:
          std::cout << " otros tipos." << std::endl;
          break;
      }

      //Creo la ventana grafica para visualizar la imagen.
      //El nombre de la ventana sirve como 'handle' para gestionarla despues.
      //Lee la documentacon de namedWindow para mas detalles.
      cv::namedWindow("IMG");

      //Visualizo la imagen cargada en la ventana.
      cv::imshow("IMG", img);

      //Para que se actualice el la interfaz grafica es necesario llamar a
      //waitKey. Además esta función nos permite interactuar con el teclado.
      //Lee la documentacion de waitKey para mas detalles.

      std::cout << "Pulsa ESC para salir." << std::endl;
      while ((cv::waitKey(0) & 0xff) != 27); //Hasta que no se pulse la tecla ESC no salimos.
   
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
