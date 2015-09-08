/**
@author Jesús Martínez Dotor
*/

#ifndef __OBJETO3D_H
#define __OBJETO3D_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "TAfin.h"
#include "RGBColor.h"

/**
 Clase Objeto3D, que contiene una TAfin y el método virtual draw, además de 2 colores.
*/
class Objeto3D{

protected:
	TAfin* _mT;

	RGBColor _color;
	RGBColor _colorlineas;
	bool _pintar_caras;
	bool _pintar_lineas;

	GLuint _textureID;

public:
	/** Constructor */
	Objeto3D() {
		_color = RGBColor(0.0, 0.1, 0.1);
		_colorlineas = RGBColor(1.0, 1.0, 0.0);
		_pintar_caras = true;
		_pintar_lineas = true;
		_textureID = 0;

		_mT = new TAfin();
	}

	/** Destructor */
	~Objeto3D() {
		//delete _mT; //este delete provoca interrupción en el cierre
		_mT = NULL;
	}

	TAfin* get_mT()	{
		return _mT;
	}

	/* Método polimorfico draw, que se implementará en las subclases */
	virtual void draw(){}

	void set_color(RGBColor c) {
		_color = c;
	}

	void set_colorlineas(RGBColor c) {
		_colorlineas = c;
	}

	void set_pintar_caras(bool b) {
		_pintar_caras = b;
	}

	void set_pintar_lineas(bool b) {
		_pintar_lineas = b;
	}

	void set_textureID(GLuint texture) {
		_textureID = texture;
	}

}; //Objeto3D

#endif // __OBJETO3D_H