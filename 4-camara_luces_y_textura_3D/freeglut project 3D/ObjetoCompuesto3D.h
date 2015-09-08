/**
@author Lin M. Dotor
*/

#ifndef __OBJETOCOMPUESTO3D_H
#define __OBJETOCOMPUESTO3D_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "Objeto3D.h"

/**
 Clase ObjetoCompuesto3D, que contiene unarray de Objeto3D.
*/
class ObjetoCompuesto3D : public Objeto3D{

protected:
	int _num_objetos; //numero de objetos
	Objeto3D** _objetos; //array de objetos

public:
	/** Constructor */
	ObjetoCompuesto3D() {	
	}

	/** Destructor */
	~ObjetoCompuesto3D() {
		delete[] _objetos;
		_objetos = NULL;
	}

	/* Método abstracto draw, que se implementará en las subclases */
	virtual void draw(){
		for(int i=0; i< _num_objetos; i++) //no se aplica la matriz del objeto compuesto principal
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			
				glMultMatrixf(_objetos[i]->get_mT()->get_m());		
				_objetos[i]->draw();

			glPopMatrix();
		}
	}

}; //ObjetoCompuesto3D

#endif // __OBJETOCOMPUESTO3D_H