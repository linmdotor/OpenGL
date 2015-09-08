/**
@author Jesús Martínez Dotor
*/

#ifndef __PLANETA_H
#define __PLANETA_H

#include <GL/freeglut.h>

#include "PV3D.h"
#include "RGBColor.h"

#define radio_planeta 100.0 //radio de la circunferencia del satelite

/**
 Clase Planeta, que almacena sus posición y el radio.
 Contiene:
	- centro -> posición del centro de la pelota
	- radio -> longitud del radio

 Operaciones: 
	- draw:  Cuadrado -> 

@author Jesús Martínez Dotor
*/
class Planeta {

private:
	PV3D _centro;
	GLdouble _radio;

	GLdouble _angulo_giro;

	int _num_meridianos; //longitudinales, de polo a polo
	int _num_paralelos; //latitudinales, que rodean la esfera

	RGBColor _color;
	RGBColor _colorlineas;

	GLUquadricObj* _quadric;

public:
	/** Constructores */
	Planeta() :
		_centro(PV3D()), _radio(radio_planeta),
		_num_meridianos(20), _num_paralelos(20), 
		_angulo_giro(0.0) , 
		_color(RGBColor(0.0, 0.1, 0.1)), _colorlineas(RGBColor(1.0, 1.0, 0.0)), 
		_quadric(gluNewQuadric()) {}

	Planeta(PV3D centro, GLdouble radio, GLdouble angulo_giro) :
		_centro(centro), _radio(radio), _angulo_giro(angulo_giro),
		_num_meridianos(20), _num_paralelos(20), 
		_color(RGBColor(0.1, 0.0, 0.1)), _colorlineas(RGBColor(1.0, 1.0, 0.0)), 
		_quadric(gluNewQuadric()) {}
	
	/** Destructor */
	~Planeta() {
		gluDeleteQuadric(_quadric);
	}

	/**	setters y getters */

	PV3D get_centro() {
		return _centro;
	}

	GLdouble get_radio() {
		return _radio;
	}

	GLdouble get_angulo_giro() {
		return _angulo_giro;
	}

	RGBColor get_color() {
		return _color;
	}

	RGBColor get_colorlineas() {
		return _colorlineas;
	}

	void set_num_meridianos(int n) {
		_num_meridianos = n;
	}

	void set_num_paralelos(int n) {
		_num_paralelos = n;
	}

	void set_color(RGBColor c) {
		_color = c;
	}

	void set_colorlineas(RGBColor c) {
		_colorlineas = c;
	}

	/**
	 Dibuja la pelota
	*/
	void draw(bool pintar_caras, bool pintar_lineas) {
		/* 
		MODOS DE PINTADO:
		GLU_FILL
			Quadrics are rendered with polygon primitives. The polygons are drawn in a counterclockwise fashion with respect to their normals (as defined with gluQuadricOrientation).

		GLU_LINE
			Quadrics are rendered as a set of lines.

		GLU_POINT
			Quadrics are rendered as a set of points

		GLU_SILHOUETTE
			Quadrics are rendered as a set of lines, except that edges separating coplanar faces will not be drawn.
		*/

		//gluQuadricTexture(_quadric_esfera, GL_TRUE);
		if(pintar_caras)
		{
			gluQuadricDrawStyle(_quadric, GLU_FILL);
			_color.changeColor();
			gluSphere (_quadric, _radio, _num_meridianos, _num_paralelos);
		}
		if(pintar_lineas)
		{
			gluQuadricDrawStyle(_quadric, GLU_LINE);
			_colorlineas.changeColor();
			gluSphere (_quadric, _radio, _num_meridianos, _num_paralelos);
		}

	}

}; //Planeta

#endif // __PLANETA_H