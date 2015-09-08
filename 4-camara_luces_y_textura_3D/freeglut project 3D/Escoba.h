/**
@author Jesús Martínez Dotor
*/

#ifndef __ESCOBA_H
#define __ESCOBA_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "ObjetoCompuesto3D.h"
#include "Hexaedro.h"
#include "DiscoParcial.h"

/**
 Clase Escoba, que contiene un semidisco y varios hexaedros como palo y cerdas.
*/
class Escoba : public ObjetoCompuesto3D{

public:
	/** Constructor */
	Escoba() {
		_num_objetos = 16; //3 para el palo, y 12 para las cerdas
		_objetos = new Objeto3D*[16];

		// palo
		_objetos[0] = new Hexaedro(1.0, 1.0, 1.0, RGBColor(0.4, 0.25, 0.1), RGBColor(0.3, 0.2, 0.1));
		_objetos[0]->get_mT()->traslada(new PV3D(100.0, 0.0, 0.0, false));
		_objetos[0]->get_mT()->escala(new PV3D(150.0, 5.0, 5.0, false));
		

		//base
		_objetos[1] = new DiscoParcial(0.0, 1.0, 15, 1, 0.0, 180.0, true, RGBColor(0.3, 0.2, 0.1), true, RGBColor(0.4, 0.25, 0.1));
		_objetos[1]->get_mT()->escala(new PV3D(30.0, 30.0, 1.0, false));
		_objetos[2] = new DiscoParcial(0.0, 1.0, 15, 1, 0.0, 180.0, true, RGBColor(0.3, 0.2, 0.1), true, RGBColor(0.4, 0.25, 0.1));
		_objetos[2]->get_mT()->escala(new PV3D(30.0, 30.0, 1.0, false));
		_objetos[2]->get_mT()->rota(180.0, new PV3D(1.0, 0.0, 0.0, false));

		//cerdas (de arriba a abajo)

		for(int i=3; i<_num_objetos; i++)
		{
			_objetos[i] = new Hexaedro(1.0, 1.0, 1.0, RGBColor(0.6, 0.4, 0.1), RGBColor(0.3, 0.2, 0.1));
			_objetos[i]->get_mT()->traslada(new PV3D(-18.0, 45.0-(i*5.0), 0.0, false));
			_objetos[i]->get_mT()->escala(new PV3D(40.0, 2.0, 2.0, false));
		}

	}

	/** Destructor */
	~Escoba() {
	}

}; //Escoba

#endif // __ESCOBA_H