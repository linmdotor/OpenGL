/**
@author Lin M. Dotor
*/

#ifndef __HEXAEDRO_H
#define __HEXAEDRO_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "Malla.h"

/**
 Clase Hexaedro, que construye un hexaedro de ciertas proporciones.
*/
class Hexaedro: public Malla{

public:
	Hexaedro(GLdouble ancho=1.0, GLdouble alto=1.0, GLdouble largo=1.0, RGBColor color=RGBColor(0.0, 1.0, 0.0), RGBColor line_color=RGBColor(1.0, 1.0, 0.0)) //con parametros por defecto
	{
		ancho = ancho/2;
		alto = alto/2;
		largo = largo/2;

		//se inicializan los Arrays de vertices, caras y normales		
		_num_vertices = 8;
		_vertice = new PV3D*[_num_vertices];		
		_num_caras = 6;
		_cara = new Cara*[_num_caras];
		_num_normales = 6;
		_normal = new PV3D*[_num_normales];

		_vertice[0] = new PV3D(-ancho, -alto , -largo, true);
		_vertice[1] = new PV3D(ancho, -alto , -largo, true);
		_vertice[2] = new PV3D(ancho, -alto , largo, true);
		_vertice[3] = new PV3D(-ancho, -alto , largo, true);

		_vertice[4] = new PV3D(-ancho, alto , -largo, true);
		_vertice[5] = new PV3D(ancho, alto , -largo, true);
		_vertice[6] = new PV3D(ancho, alto , largo, true);
		_vertice[7] = new PV3D(-ancho, alto , largo, true);

		int arrayVN0[4][2] = {{0, 0}, 
							{1, 0}, 
							{2, 0}, 
							{3, 0}};

		int arrayVN1[4][2] = {{0, 0}, 
							{4, 0}, 
							{5, 0}, 
							{1, 0}};

		int arrayVN2[4][2] = {{1, 0}, 
							{5, 0}, 
							{6, 0}, 
							{2, 0}};

		int arrayVN3[4][2] = {{2, 0}, 
							{6, 0}, 
							{7, 0}, 
							{3, 0}};

		int arrayVN4[4][2] = {{3, 0}, 
							{7, 0}, 
							{4, 0}, 
							{0, 0}};

		int arrayVN5[4][2] = {{7, 0}, 
							{6, 0}, 
							{5, 0}, 
							{4, 0}};

		_cara[0] = new Cara(4, arrayVN0);
		_cara[1] = new Cara(4, arrayVN1);
		_cara[2] = new Cara(4, arrayVN2);
		_cara[3] = new Cara(4, arrayVN3);
		_cara[4] = new Cara(4, arrayVN4);
		_cara[5] = new Cara(4, arrayVN5);
		
		calcularNormales();

		set_color(color);
		set_colorlineas(line_color);
	}

}; //Hexaedro

#endif // __HEXAEDRO_H