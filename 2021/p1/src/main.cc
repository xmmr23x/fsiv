/**
 * @author Manuel Mariscal
 * @date 29/09/2020
 * @file main.cc
 * @brief Realce de una imagen seleccionando un rectangulo o un poligono.
 * @version 1.0
 */

#include "funciones.h"

const cv::String keys =
	"{help h usage ? |      | print this message   }"
	"{p 			 |      | ejecuta onMousePoligono   }"
	"{@image1        |      | image1 for compare   }"
	"{@image2        |<none>| image2 for compare   }"
	;


/**
 * @fn int main(int argc, char* const* argv)
 * @brief Cuerpo del programa.
 * @pre Se debe indicar la ruta y el nombre de una imagen origen y otra destino para guardar los resultados.
 *
 * El programa recibirá como argumento la ruta de la imagen en la que se desea realizar el realce y la ruta donde se guardará el resultado en caso de ser el esperado. Hay dos opciones de ejecución:
 * @li Opción por defecto: permite realizar el realce seleccionando dos puntos de la imagen o seleccionando un area de forma interactiva. Una vez realizado el realce no se puede modificar.
 * @li -p: Permite seleccionar varios puntos de forma que se genere un poligono. Una vez realizado el realce no se puede modificar.
 */
int main (int argc, char* const* argv) {
	int retCode=EXIT_SUCCESS;

	try {    

		cv::CommandLineParser parser(argc, argv, keys);
		parser.about("Application name v1.0.0");
		if (parser.has("help")) {
			parser.printMessage();
			return 0;
		}
		
		cv::String img1 = parser.get<cv::String>("@image1");
		cv::String img2 = parser.get<cv::String>("@image2");
		
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}

		
		cv::Mat imagenes[2];
		imagenes[0] = cv::imread(img1, cv::IMREAD_COLOR);

		if (imagenes[0].empty()) {
			std::cerr << "Error: no he podido abrir el fichero '" << img1 << "'." << std::endl;
			return EXIT_FAILURE;
		}

		// creo la ventana
		cv::namedWindow("IMG");

		// visualizo la imagen
		cv::imshow("IMG", imagenes[0]);

		if (parser.has("p"))
			cv::setMouseCallback("IMG", onMousePoligono, (void *)&imagenes);
		else
			cv::setMouseCallback("IMG", onMouse, (void *)&imagenes);

		// Si se recibe una entrada distinta a ESC se guarda la imagen.
		if (cv::waitKey(0) != 27)
			cv::imwrite(img2, imagenes[1]); 

	}
	
	catch (std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		retCode = EXIT_FAILURE;
	}
	
	return retCode;
}
