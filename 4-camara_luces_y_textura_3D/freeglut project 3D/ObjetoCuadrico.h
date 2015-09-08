/**
@author Lin M. Dotor
*/

#ifndef __OBJETOCUADRICO_H
#define __OBJETOCUADRICO_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "Objeto3D.h"

/**
 Clase ObjetoCuadrico.
*/
class ObjetoCuadrico : public Objeto3D{

protected:
	GLUquadricObj* _quadric;

public:
	/** Constructor */
	ObjetoCuadrico() : 
	  _quadric(gluNewQuadric()) {}
	
	/** Destructor */
	~ObjetoCuadrico() {
		gluDeleteQuadric(_quadric);
	}

}; //ObjetoCuadrico

#endif // __OBJETOCUADRICO_H