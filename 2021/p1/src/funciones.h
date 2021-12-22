/**
 * @author Manuel Mariscal
 * @date 29/09/2020
 * @file funciones.h
 * @brief 
 * @version 1.0
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <vector>
#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * Permite la selección de un rectangulo de dos formas distintas:
 * @li Seleccionando dos puntos sobre la imagen.
 * @li De forma interactiva arrastrando el ratón.
 * Una vez seleccionado el rectangulo, no se podrá cambiar la seleccion.
 * 
 * @param event    Recibe un evento desde el ratón. Pueden ser de movimiento o pulsación de botones.
 * @param x        Coordenada x del evento del ratón.
 * @param y        Coordenada y del evento del ratón.
 * @param flags    
 * @param imagenes Vector con las imagenes de entrada y salida.
 */
void onMouse(int event, int x, int y, int flags, void *imagenes);

/**
 * Permite la selección de un poligono seleccionando varios puntos con el ratón. Una vez seleccionado el rectangulo, no se podrá cambiar la seleccion.
 * 
 * @param event    Recibe un evento desde el ratón. Pueden ser de movimiento o pulsación de botones.
 * @param x        Coordenada x del evento del ratón.
 * @param y        Coordenada y del evento del ratón.
 * @param flags    
 * @param imagenes Vector con las imagenes de entrada y salida.
 */
void onMousePoligono(int event, int x, int y, int flags, void *imagenes);

/**
 * Cambia a escala de grises los colores de los pixeles externos al rectángulo recibido y guarda el resultado en una matriz imagen por si es necesario almacenarla.
 * @param mask     Mascara para transformar la imagen.
 * @param imagenes Vector de dos elementos con las imagenes.
 */
void setGrises(cv::Mat mask, cv::Mat *imagenes);


#endif