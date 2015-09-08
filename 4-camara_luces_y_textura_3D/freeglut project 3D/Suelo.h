/**
@author Jesús Martínez Dotor
*/

#ifndef __SUELO_H
#define __SUELO_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "ObjetoCompuesto3D.h"
#include "RGBColor.h"
#include "Hexaedro.h"

/**
 Clase Suelo, que contiene un hexaedro.
*/
class Suelo : public ObjetoCompuesto3D{

public:
	/** Constructor */
	Suelo() {
		_num_objetos = 1;
		_objetos = new Objeto3D*[1];

		//suelo
		_objetos[0] = new Hexaedro(); 
		_objetos[0]->set_color(RGBColor(1.0, 1.0, 1.0));
		_objetos[0]->set_pintar_lineas(false);
		_objetos[0]->set_textureID(2); //TEXTURA_WOOD = 1
		_objetos[0]->get_mT()->escala(new PV3D(250.0, 10.0, 250.0, false));
	}

	/** Destructor */
	~Suelo() {
	}

}; //Suelo

#endif // __SUELO_H