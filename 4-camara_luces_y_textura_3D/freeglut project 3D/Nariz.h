/**
@author Jesús Martínez Dotor
*/

#ifndef __NARIZ_H
#define __NARIZ_H

#include <GL/freeglut.h>

#include "RGBColor.h"
#include "Cilindro.h"


/**
 Clase Nariz, sobre la que se puede hacer click, ya que le asigna un nombre en el draw(). 
*/
class Nariz : public Cilindro{

private:
	int num_objeto;

public:
	/** Constructores */
	Nariz(int n) : Cilindro(0.5, 0.0, 3.0, 15, 1, true, RGBColor(1.0, 0.5, 0.0), false, RGBColor(0.0, 0.0, 0.0)) {
		num_objeto = n;
	}

	/**
	 Dibuja el Cilindro
	*/
	void draw() {	
		glPushName(num_objeto);
			Cilindro::draw();
		glPopName();
	}

}; //Nariz

#endif // __NARIZ_H