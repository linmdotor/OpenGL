/**
@author Jesús Martínez Dotor
*/

#ifndef __OJO_H
#define __OJO_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "ObjetoCompuesto3D.h"
#include "Cilindro.h"
#include "Esfera.h"

/**
 Clase Ojo, que contiene un cilindro y un foco direccional.
*/
class Ojo : public ObjetoCompuesto3D{

private:
	int _luz; //indice de la luz GL_LIGHT

public:
	/** Constructor */
	Ojo(int luz) {
		_luz = luz;

		_num_objetos = 2;
		_objetos = new Objeto3D*[2];

		// ojo
		_objetos[0] = new Cilindro(0.5, 0.5, 0.5, 15, 1, true, RGBColor(0.0, 1.0, 0.0), true, RGBColor(0.0, 0.0, 0.0));
		
		//esfera para que se vea el efecto
		_objetos[1] = new Esfera(2.0, 15, 15, true, RGBColor(0.0, 0.0, 1.0), true, RGBColor(0.0, 0.0, 0.0));
		_objetos[1]->get_mT()->traslada(new PV3D(0.0, 0.0, 7.0, false));

		// LUZ FOCO
		GLfloat d[] = {1.0, 1.0, 1.0, 0.0};
		GLfloat zero[] = {0.0, 0.0, 0.0, 0.0};

		glEnable(_luz);
		glLightfv(_luz, GL_DIFFUSE, d);
		glLightfv(_luz, GL_AMBIENT, zero);
		glLightf(_luz, GL_SPOT_CUTOFF, 10.0);
		glLightf(_luz, GL_SPOT_EXPONENT, 4.0);

	}

	/** Destructor */
	~Ojo() {
	}

	void draw(){
		// LUZ FOCO
		GLfloat p[] = {0.0, 0.0, -0.5, 1.0};	
		GLfloat dir[]={0.0, 0.0, 1.0};

		glLightfv(_luz, GL_POSITION, p);
		glLightfv(_luz, GL_SPOT_DIRECTION, dir);

		for(int i=0; i< _num_objetos; i++)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();			
				glMultMatrixf(_objetos[i]->get_mT()->get_m());		
				_objetos[i]->draw();
			glPopMatrix();
		}

		
	}

}; //Ojo

#endif // __OJO_H