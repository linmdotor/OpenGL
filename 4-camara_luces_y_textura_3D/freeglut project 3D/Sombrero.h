/**
@author Lin M. Dotor
*/

#ifndef __SOMBRERO_H
#define __SOMBRERO_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "ObjetoCompuesto3D.h"
#include "Cilindro.h"
#include "Disco.h"

/**
 Clase Sombrero, que contiene un cilindro y un disco.
*/
class Sombrero : public ObjetoCompuesto3D{

public:
	/** Constructor */
	Sombrero() {
		_num_objetos = 3;
		_objetos = new Objeto3D*[3];

		// tubo
		_objetos[0] = new Cilindro(0.5, 0.5, 3.0, 15, 1, true, RGBColor(0.2, 0.2, 0.2), true, RGBColor(1.0, 1.0, 1.0));
		_objetos[0]->get_mT()->escala(new PV3D(30.0, 30.0, 10.0, false));

		//base
		_objetos[1] = new Disco(0.5, 1.0, 15, 1, true, RGBColor(0.0, 0.0, 0.0), true, RGBColor(1.0, 1.0, 1.0));
		_objetos[1]->get_mT()->escala(new PV3D(30.0, 30.0, 1.0, false));

		//tapa
		_objetos[2] = new Disco(0.0, 0.5, 15, 1, true, RGBColor(0.0, 0.0, 0.0), false, RGBColor(1.0, 1.0, 1.0));
		_objetos[2]->get_mT()->traslada(new PV3D(0.0, 0.0, 30.0, false));
		_objetos[2]->get_mT()->escala(new PV3D(30.0, 30.0, 1.0, false));

	}

	/** Destructor */
	~Sombrero() {
	}

}; //Sombrero

#endif // __SOMBRERO_H